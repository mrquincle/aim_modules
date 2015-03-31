# Intro

The nodejs module can be used in a standalone server, but you will need to implement all interfaces to the outside world yourself. This can go fast with e.g. using a framework like Express, etc. However, there are also software frameworks in place that are meant for heterogeneous distributed computation, called multi-agent systems.

The implementation in this folder makes use of evejs, of which the documentation can be found at https://github.com/enmasseio/evejs.

# Usage

Build of course first the module using the `nodejs` backend.

Run the following in this folder:

    npm install evejs promise

Run the following in one terminal:

    node start.js

Run the following in a second terminal:

    node request.js

That's it!
