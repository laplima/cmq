CORBA MESSAGE QUEUES - CMQ
(C) 2019 by Luiz Lima Jr. All rightes reserved.

This is a simplified implementation of AMQP in CORBA (RabbitMQ is the standard implementation).
See: http://www.rabbitmq.com/tutorials/amqp-concepts.html for an overview of the protocol.

A `Connection` is a `Channel` factory.
A `Channel` is used to create `Exchanges` and associated `Queues`.

Components:

    - `cmq_server` implements exchanges, queues and bindings.
    - `cmq_client` plays both producer and consumer roles.
    - `cmq_producer` (to do)
    - `cmq_consumer` (to do)
