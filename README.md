# CORBA MESSAGE QUEUES - CMQ
## (C) 2019-23 by Luiz Lima Jr. All rights reserved.

This is a simplified implementation of AMQP in CORBA (RabbitMQ is the standard implementation).

See [AMQP Concepts](http://www.rabbitmq.com/tutorials/amqp-concepts.html) for an overview of the protocol.

## Details

* A `Connection` is a `Channel` factory.
* A `Channel` is used to call operations on the broker (e.g., create `Exchanges`
  and associated `Queues`).

### Components:

* the `broker` implements exchanges, queues, and bindings.
* the `client` plays both producer and consumer roles.
* `publisher` (to do)
* `consumer` (to do)
