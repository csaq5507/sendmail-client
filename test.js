const SendMail = require('bindings')('sendmail.node')

console.log("Open mail to ivan@waldboth.com: ", SendMail.send("ivan@waldboth.com", "SendMail Node", "Hello Ivan Waldboth,\nthank you for this awesome package.\nI have to ask/say following:\n\n...\n\nBest regards\n...") ? "success" : "failure");
