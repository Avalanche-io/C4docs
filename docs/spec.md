# The C4 Interface Specification

## Introduction

The C4 interface defines a set of primitives for interacting with all components of a cloud production pipeline.  This includes raw assets, attributes, groups, processes, transport channels, resource contexts, identity, security and other aspects.

These primitives are intended to be used in a variety of ways including immediate mode command line tools, language bindings for existing programming languages, C4’s own domain language, and application level frameworks.

This means the C4 interface can be used to directly manipulate assets within a specific production, and can also be use to create general purpose applications for working on any production.

### Motivations

The primary idea behind the C4 interface is to reframe command and control of scalable local and remote resources in a media production centric way.  This allows production pipelines and applications to be built quickly and easily, while automatically including services that can scale as needed with production demand.

It is, in effect, a layer above infrastructure stacks such as OpenStack and unifies multiple service endpoints that could include SaaS and IaaS providers for different aspects of production work flow.


### Features

Computable asset identities allow assets that are often separated from notes and follow multiple paths through productions to be reliably reconnected at any point in the production process.  A video file found on a USB key with no identifying information for example, can be uniquely identified and linked to any data known by the production about the given asset id.

The id system also allows applications to work on assets without regard to storage paths or other facility specific infrastructure choices, reducing errors related to improperly specified paths, broken links, network outages etc.

Attributes allow assets to be found like database records instead of by fixed file path as is common now.  In production it is often important to find assets based on Director selection, date, keyword or other parameters that can be searched.  The C4 Interface does not require a user to have _a prioi_ knowledge of a file system structure such as "/show/seq/shot/element". 

A robust grouping system allows a variety of grouping features from version control, to mimicking a traditional file system.

Entities provide a richer collection of identity types then simple user account authentication.  Complicated access management interactions can be described not only between users, but also organizations, agents, and other entities.

Processes can be connected to establish fixed or malleable work flows that include everything from color correcting images, to automatically moving assets between physical locations, and can even represent human processes providing end to end tracking of asset development.

Strong cryptography provides a security layer that offers features including end to end encryption, "at rest" encryption, data signing, revocable access rights, and tiered certificate authorities amongst many other useful features. 

--

## Assets

Assets are core data types that represent the media upon which the rest of the interface operates.  The obvious examples are images, video and audio, but any data can be considered an asset for a particular application.  

An asset as only three values “id”, “type”, and “incept”.

**id** is a sha256 hash of the media.

**type** is the mime type of the asset.

**incept** is the creation time stamp.

Any additional information that is _required_ to read the asset must be contained as parameters with in the mime type.  For example image formats that do not contain line width information, etc.  

### Id Representations

Ids have two different representations the "absolute id" which is the complete base64 representation of the sha256 id.  And the "C4 id" which is a base58 number which starts with c4 followed by 1 or more base58 digits.  The C4 id need only use as many digits as is required to uniquely identify an existing asset id in the current context _(Resource Contexts tbd)_, so is typically very short, whereas the absolute id must always be used in it's entirety.

## Attributes

Additional information can be associated with assets via **attributes**.  Attributes can be defined as per asset or per type.

### Per Type Attributes

Per type attributes are implicitly defined for all assets, like a column in a database. Once created they exist for all assets of the given type within the current context. 

### Per Asset Attributes

Per asset attributes are associated with one and only one asset within the current context. 

### Attribute Data Types

Attribute data types are still in the process of being defined, but these are the core data types that are currently proposed:

- boolean
  - true / false
- integer
  - Signed integer values
- real
  - Floating point values
- string
  - String values
- timestamp
  - Date with or without time and timezone
- var
  - Untyped value (i.e. any type / void)
- json
  - Json structured data
- binary
  - Arbitrary binary data
- arrays 
  - Arrays of any of the above types. 

## Groups

Groups define relationships between assets, or other groups.

### Group Types

The currently defined list of predefined groups are.

  - Collection
    + A hierarchical group of collections, revisions or assets in which each collection represents _all_ of the assets contained within itself and all child collections.
  - Folder
    + A typical filesystem hierarchy, in which folders contain folders, revisions or assets, but only represents immediate children.
  - Tag
    + “/“ delimited hierarchy of keys that explicitly refer to a specific set of assets, or revisions.  Assets can be used to fined the set of tags, tags can be used to find the set of assets.
  - Archive
    + A group of any other group type plus assets.
  - Clip
    + A sequential group of assets, or clips. 
    + Clips have two modes of operation
      - Sequential
        Start to end with optional step.
        `[0,1,2,3,4,5,6,7,8]`
      - Binary
        Start to end with optional minimum step and automatic binary step devision. So starting with highest binary step (in this case 8), and each step / 2 until minimum step is reached.
        `[0,8,4,2,6,1,3,5,7]`
  - Revision
    - A group of sequential versions of a single asset, or any other single group type other than revision.

## Entities

Entities represent user accounts, companies, agents and other types of 'actors' who can given various rights with respect to any interface component including other entities.

## Processes

Processes represent the various actions that should be performed on interface components.  This includes data movement, data processing, human tasks, or anything else that functionally 'transforms' a component.

## License

The C4 Interface is released under the Apache 2.0 license.  For other licensing options contact Studio Pyxis.

This document, and the C4 Interface are Copyright 2014 Joshua Kolden and Studio Pyxis.

Licensed under the Apache License, Version 2.0 (the "License").  See LICENSE file for more information.
