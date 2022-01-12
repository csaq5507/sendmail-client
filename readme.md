# sendmail-client

Simple package that allows you to send mails using your standard Mail Application.
In contrast to the mailto link this package supports an attachment.
Just developed for own needs, if you may request more features i'll implement them. ;)


Calling send will open a "New Email" window of your standard Mail application installed on your device.

Caution!! Atm only for Windows.

## Requirements

To install, you will need:

- node-gyp


## Installation

``
yarn add senmail-client
``

```
import SendMail from 'sendmail-client';

SendMail.send('ivan@waldboth.com','Hello World','<html><body><h1>Hello World</h1><p>This is a test!</p></body></html>');
```



## Doc
```
SendMail(recipient: string, subject: string, body: string, attachment?:string):bool
```

``body``: HTML or plain.
``attachment``: full path to file.
