# Choose a Tech Stack! :)

Make sure to complete the steps in the Lab where you develop the code in the ```chat``` folder. This will give you a little hands-on experience working with everything.

# Your Team

You are the lead developers at a small Dev House, where you constantly build full stack apps for clients. These are primarily CRUD apps with user authentication and public APIs. You are trying to pick a set of reliable and reusable technologies to solidify your company's tech stack.

This will help you reuse code across clients and ensure timely deliver of projects per client. 

Your team members expertise and interest across different technologies:

| Team Member | Technology | Expertise | Interest |
| ----------- | ---------- | --------- | -------- |
| Lilly       | Javascript | Low   | Medium |
| Jordi       | Javascript | Low   | Low |
| Angel       | Javascript | High  | Low |
| Lilly       | C++        | High  | Medium |
| Jordi       | C++        | High  | High |
| Angel       | C++        | High  | Low |
| Lilly       | Rust       | Low   | Low |
| Jordi       | Rust       | Low   | High |
| Angel       | Rust       | Low   | High |
| Lilly       | Phoenix    | Low    | High |
| Jordi       | Phoenix    | Medium | High |
| Angel       | Phoenix    | Low    | Medium |
| Lilly       | HTMX       | Low    | Low |
| Jordi       | HTMX       | Low    | Medium |
| Angel       | HTMX       | Medium | Medium |

Overall your team have strong development experience using server-side technologies like C++ and strong interest in C++ and Rust. In general, they do not like client-side technologies like Javacript, and likely HTML/CSS. How can you select technologies and frameworks that will help your team be happy and productive while delivery high quality solutions for your customers?


# Technology & Framework Research

Read up on the history of the the C++ Web Framework Crow. Are there any better alternatives?  Compare your technology stack ideas with your team's expertise and interests. 

#### Make the case for using the C++ Web Framework Crow
Two strong points supporting using Crow in your Server-Side Stack (2-3 sentences per point).

1. Crow is exceptionally fast. Load times will still be short even when many users are accessing the app concurrently.

2. The team already has a high level of C++ expertise. This will cut down on the amount of development time spent on learning a new language, thus reducing the app's time to market.


#### Make the case for NOT using the C++ Web Framework Crow
Two strong points supporting NOT using Crow in your Server-Side Stack (2-3 sentences per point).

1. The web is a volatile environment, and its content tends to be quite dynamic. This may not mesh well with a statically-typed language like C++. If the app relies heavily on dynamically-generated content, the backend code might require a lot of type conversions and other boilerplate code.

2. C++ is a relatively old language. As technology continues to evolve, existing C++ code will become harder to maintain. As such, it might be worth the investment to learn a newer language that the team is interested in, like Elixir for Phoenix or Rust.


# Technology & Framework Decision

### Describe your Client-Side Stack?
Describe each part below in 3--5 sentences. Make sure to include possible positives and negatives of your choices. 

##### Describe the Teck Stack (frameworks/languages):

The team's client-side stack will be built upon the HEEx templating language. It integrates tightly with the Phoenix framework and allows developers to generate web pages much more programmatically than with plain HTML and JS.

##### Describe the Network Protocols this tech uses:

As with most templating software, the rendering is done on the server based on the data requested by the client. The resulting HTML page is then sent back and rendered by the client's web browser. As such, the front end relies only on HTTPS (port 443), making it lightweight and accessible.

##### How does this match your team's interest and expertise?

The team has expressed a distaste for traditional front-end technologies, including JavaScript. Using a templating language like HEEx greatly reduces the amount of JS needed to render functional and elegant web pages.

### Describe your Server-Side Stack?
Describe each part below in 3--5 sentences. Make sure to include possible positives and negatives of your choices.

##### Describe the Teck Stack (frameworks/languages):

The team's server-side stack will include Phoenix, an Elixir-based web framework. While its syntax may come with a bit of a learning curve for the team, the flexibility of Elixir is much greater than that of other languages like Rust, enhancing its reusability. The database will be built upon PostgreSQL, a popular and well-documented SQL database technology. The team can use the Ecto library to easily interact with the database from within the Phoenix code.

##### Describe the Network Protocols this tech uses:

According to its website, PostgreSQL uses a proprietary, message-based protocol on TCP port 5432. The protocol is robust, supporting authentication handshakes as well as automatic rollback upon query errors.

##### How does this match your team's interest and expertise?

The team is generally highly interested in Phoenix, which makes it a good choice for future development. The team does not have a lot of experience with the framework, however, so a gradual adoption may be the ideal choice.