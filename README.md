# C4 - The Cinema Content Creation Cloud

[![Stories in Ready](https://badge.waffle.io/etcenter/c4.png?label=ready&title=Ready)](https://waffle.io/etcenter/c4)

C4 the **Cinema Content Creation Cloud** is an open source framework for content creation using cloud resources, or in other words remote resources that come and go. Originally created by Studio Pyxis it is now open source and part of the Entertainment Technology Center's "Production in the Cloud" project serving as the "framework" component of the project.

# Background
The [Entertainment Technology Center @ the University of Southern California (ETC)](http://www.etcenter.org) has launched “Production in the Cloud,” a new project that brings together a core group of key media and cloud-resource leaders to develop guidelines and accelerate innovation and adoption of next-gen cloud-based content creation, production, and distribution tools and processes. Senior executives from the six major studios in coordination with Rackspace, EMC, EVault, Front Porch Digital, Google and many others serve as governing body to collectively guide this process. The project is looking at the life cycle of film and media production, from pre-production collaboration, production, post production and through to archiving.

The framework is provided in this github repository.  It is designed to improve interoperability, describe workflows, and define security requirements allowing organizations, services and individuals to easily collaborate on production.

An introductory slide deck is available at [SlideShare](http://www.slideshare.net/JoshuaKolden/c4-framework-41598520).

The framework is still in development, and we encourage anyone to help improve the framework by participating and contributing.

--- 

- [The Framework Components](#The_Framework_Components)
  - [Interoperability](#Interoperability)
  - [Language](#Domain_Language)
  - [Assets](#Asset_Identification)
  - [Access and Identity](#Access_and_Identity)

## The Framework Components

### Interoperability
The C4 framework provides a low impact way for software and services to become interoperable with with other C4 services.  

This is done by creating a C4 Language document that describes the service API, and by providing the API interface via the C4 DSL, Yaml, or JSON (there may be other format translators, such as XML in the future).

Detailed documentation on how to do this is on it's way very shortly.

### Language
C4 provides a Domain Specific Language (DSL) based on YAML (a superset of JSON) to describe metadata, service APIs, and workflows.  The C4 DSL makes it much easier to describe and reason about production dependencies by abstracting the lower levels of security, transport, asset identification, and resource orchestration.

C4 adds inline markdown based comments and documentation to YAML, removes some superfluous punctuation to further improve readability, and adds the ability to define executable code in almost any programming language to be run locally or remotely.  

For more check out the [C4 Language docs](https://github.com/etcenter/C4/blob/master/docs/language.md) 

### Assets
The problem: Interoperable identification of media assets across different organizations, regardless of differences in naming conventions and without prior agreement.

The solution: C4's asset identification system is based on a [cryptographic hash function](http://en.wikipedia.org/wiki/Cryptographic_hash_function) of the asset itself.  A C4 asset ID is the [SHA256](http://en.wikipedia.org/wiki/SHA-2) hash of the asset.

This hash, or "message digest" can be easily computed by any user with tools that are already available in most operating systems.  It is immutable, meaning that a given asset will **always** generate the same ID, and it is a *very large* number space meaning that no two assets will ever get the same ID.

This ID system avoids the need for a central authority or registry to issue ids or even the need to have a network connection. Identifying assets in the fashion makes it much easier to coordinate between various production units, databases, online services.  

Some other benefits of the C4 Asset ID system:

- Data De-duplication
- Transport Optimization 
- Data Integrity Verification
- Data Signing

For more see the [C4 Asset ID docs](https://github.com/etcenter/C4/blob/master/docs/assets.md)

### Access and Identity
The Framework provides a [public key infrastructure]() based access control system that allows for both distributed and federated access management.  Any individual or organization can be easily added or removed from access to projects data across multiple domains.

The C4 Framework prescribes that **all** network communication is encrypted.  Both server and client are authenticated, and access can be controlled at the individual level across all participating domains.

To achieve this the C4 security system is based on [public key cryptography](http://en.wikipedia.org/wiki/Public-key_cryptography) and a [certificate authority](http://en.wikipedia.org/wiki/Certificate_authority) chain based on the same technology that secures the Internet.

Users have one or more private keys which they use to authenticate to C4 services. Private keys are kept secure by each user, and never transmitted to the server instead C4 language files are 'signed' with the private key and verified on the other end using the public key. 

This can happen in either direction, users can verify the service, and the service can verify the user. In addition to this the certificate system insures provides the ability to grant rights to data and withdraw rights at anytime across all domains.

For example.  A studio might be a certificate authority for a given project.  They then grant rights to a production company to not only to access the production data, but also grant similar rights to it's employees. 

Now regardless of where the data resides a user who has a valid certificate from the production company can access the data.  The service providing the data need only know the studio's certificate root certificate.  Vice versa the user can verify that the service also has a certificate signed by the studio insuring that both have access to the data.

If the user leaves the project, the production company or studio can [revoke](http://en.wikipedia.org/wiki/Revocation_list) the user's certificate instantly removing the users access across all domains.  In fact even an entire organization could be easily revoked from the certificate chain causing that organization and all sub-orginizations or users to be revoked.

## Contributers

[Joshua Kolden](https://github.com/JoshuaKolden) - Framework Architect

## Project Plans

This project it intended to outline the the basic requirements for the C4 Framework, provide a reference client implementation, execution environment, language bindings, and a test framework to drive client development and evaluate interface compliance in new implementations.

## Contributing

Contribution is encouraged from both programmers and non programmers who have expertise to share. 

Please add an [issue](https://github.com/JoshuaKolden/C4Interface/issues) if you have ideas, or fork the project and implement code that satisfies test requirements (as more tests are setup).

## License

This document, and the C4 Framework are Copyright 2014 Joshua Kolden and Studio Pyxis.

Licensed under the Apache License, Version 2.0 (the "License").  See LICENSE file for more information.
