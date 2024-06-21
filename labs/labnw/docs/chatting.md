# Chatting

Please answer the following questions. For each question please provide 3--5 sentences.

### Does the code behave like a normal ChatApp? (desribe why or why not)

This program functions differently from traditional chat apps in a few ways:

- There is no login function, you just pick a name and start sending messages
- Anyone can choose any username, even if it is already in use
- You can't choose who to send a message to, it is simply broadcasted to everyone

### Describe why you prefer the JS/WebSocket or the HTMX implementation?

I can appreciate the idea of minimizing the amount of JS code in the app by using HTMX.
However, I think WebSockets are the way to go for an app that depends on live, bidirectional communication.
They're responsive and relatively easy to set up.
I'm sure there's a way you could get a functional chat app running using HTMX, but I think being forced to use HTTP methods to send and receive messages would make it more difficult than it needs to be.

### Why does our Crow App use port 9095? (this is a tricky one, think of your dev/server environment)

As far as I can tell, port 9095 is an arbitrary choice.
In a way, all port numbers are arbitrary.
There is nothing stopping you from making a chat app that uses port 8080, even if doing so would be a bad idea because, by convention, that port is only used for HTTP traffic.
As long as the client and server both agree on the port number, the number itself doesn't matter.