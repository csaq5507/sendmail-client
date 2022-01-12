#include <napi.h>
#include <algorithm>

#ifdef _WIN32

#include <windows.h>
#include <mapi.h>

#endif


Napi::Value Send(const Napi::CallbackInfo& info) {
#ifndef _WIN32
    return Napi::Boolean::New(env,false);
#endif

    HINSTANCE hMAPI = ::LoadLibrary("mapi32.dll");
    auto lpfnMAPISendMail = (LPMAPISENDMAIL) ::GetProcAddress(hMAPI, "MAPISendMail");
    Napi::Env env = info.Env();
    if (info.Length() < 3) {
    Napi::TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return env.Null();
    }
    if (!info[0].IsString() || !info[1].IsString() || !info[2].IsString()) {
      Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
      return env.Null();
    }
    std::string recipient = info[0].As<Napi::String>().Utf8Value();
    std::string subject = info[1].As<Napi::String>().Utf8Value();
    std::string body = info[2].As<Napi::String>().Utf8Value();
    std::string attachment = "";
    if(info.Length() > 3){
        attachment = info[3].As<Napi::String>().Utf8Value();
    }

char szTo[MAX_PATH] = {0};
    strcat_s(szTo, (LPCSTR) recipient.c_str());

    MapiRecipDesc recipients[1] = {0};
    recipients[0].ulRecipClass = MAPI_TO;
    recipients[0].lpszAddress = szTo;

    char szSubject[MAX_PATH] = {0};
    strcat_s(szSubject, (LPCSTR) subject.c_str());

    char szText[MAX_PATH] = {0};
    strcat_s(szText, (LPCSTR) body.c_str());
    MapiMessage MAPImsg = {0};

    if (!attachment.empty()) {
        MapiFileDesc fileDesc;
        ZeroMemory(&fileDesc, sizeof(fileDesc));
        fileDesc.nPosition = (ULONG) 0xFFFFFFFF;
        auto pos = attachment.find_last_of('/') + 1;
        auto fileName = attachment.substr(pos, attachment.length() - pos);
        auto path = attachment;
        std::replace(path.begin(), path.end(), '/', '\\');
        fileDesc.lpszPathName = (LPTSTR) path.c_str();
        fileDesc.lpszFileName = (LPTSTR) fileName.c_str();
        MAPImsg.nFileCount = 1;
        MAPImsg.lpFiles = &fileDesc;
        MAPImsg.lpszSubject = szSubject;
        MAPImsg.lpRecips = recipients;
        MAPImsg.nRecipCount = 1;
        MAPImsg.lpszNoteText = szText;

        ULONG nSent = lpfnMAPISendMail(0, 0,
                                       &MAPImsg, MAPI_LOGON_UI | MAPI_DIALOG | MAPI_NEW_SESSION, 0);

        return Napi::Boolean::New(env,(nSent == SUCCESS_SUCCESS || nSent == MAPI_E_USER_ABORT));
    }

    MAPImsg.lpszSubject = szSubject;
    MAPImsg.lpRecips = recipients;
    MAPImsg.nRecipCount = 1;
    MAPImsg.lpszNoteText = szText;

    ULONG nSent = lpfnMAPISendMail(0, 0,
                                   &MAPImsg, MAPI_LOGON_UI | MAPI_DIALOG | MAPI_NEW_SESSION, 0);

    return Napi::Boolean::New(env,(nSent == SUCCESS_SUCCESS || nSent == MAPI_E_USER_ABORT));
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "send"), Napi::Function::New(env, Send));
    return exports;
}

NODE_API_MODULE(addon, Init)
