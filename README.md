# CORBA MESSAGE QUEUES - CMQ
## (C) 2019-21 by Luiz Lima Jr. All rightes reserved.

This is a simplified implementation of AMQP in CORBA (RabbitMQ is the standard implementation).

See: http://www.rabbitmq.com/tutorials/amqp-concepts.html for an overview of the protocol.

A `Connection` is a `Channel` factory.
A `Channel` is used to call operation on the broker (e.g, create `Exchanges`
and associated `Queues`).

Components:

    - `broker` implements exchanges, queues and bindings.
    - `client` plays both producer and consumer roles.
    - `producer` (to do)
    - `consumer` (to do)
