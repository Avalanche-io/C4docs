# The C4 Language

## Introduction

The C4 DSL defines a set of primitives for interacting with all components of a cloud production pipeline.  This includes raw assets, attributes, groups, processes, transport channels, resource contexts, identity, security and other aspects.

These primitives are intended to be used in a variety of ways including immediate mode command line tools, language bindings for existing programming languages, C4’s own domain language, and application level frameworks.

This means the C4 DSL can be used to directly manipulate assets within a specific production, and can also be use to create general purpose applications for working on any production.

### Motivations

The primary idea behind the C4 DSL is to reframe command and control of scalable local and remote resources in a media production centric way.  This allows production pipelines and applications to be built quickly and easily, while automatically including services that can scale as needed with production demand.

It is, in effect, a layer above infrastructure stacks such as OpenStack and unifies multiple service endpoints that could include SaaS and IaaS providers for different aspects of production work flow.


# The Language

The C4 DSL is a superset of YAML, and therefore YAML and JSON interfaces are also comparable with C4. The C4 language is a declarative language, meaning we describe the world as we would like it to appear, and it is up to the implementation to make it so.  This is different than procedural languages like C or JavaScript in which results are achieved by describing each step of a process.

C4 adds inline Markdown based comments and documentation to YAML, removes some superfluous punctuation to further improve readability, and adds the ability to define executable code in almost any programming language.

As with YAML and JSON in it's simplest form C4 simply describes data. In effect the C4 language is just a JSON object or a map/dictionary/hash/structure (whatever term you prefer) of values or sub objects.  So for example:

```c4
key: “value1"

key2: { sub_key: “value 2", sub_key2: “value 3" } 

key3: [ “array item 1”, “array item 2” ]

key4: { “key”: key, “key2”: key2, “key3”: key3 }
```

One can continue to nest key values as deep as you like.  For ease of human use we can eliminated most the the punctation noise with convention and formatting, so writing out the complete `key4` object above in the C4 language would be:

```c4
key4
  key
    key
      value1
  key2
    sub_key
      value 2
    sub_key2
      value 3
  key3
    array item 1
    array item 2
```

This is similar to YAML, but with even fewer punctuation symbols. As you inline keys and values, or in ambiguous situations you have to fall back to using the punctuation as shown the first example.

The real meat of the language is the idea that it represents a dependency graph in which one starts at the base with the desired result, so in the example above `key4` is the desired result and `key`,  `key2`, and `key3` are “dependancies” of `key4`.  

So you might say something like this:

```c4
Movie
  edit
    footage
      script
```

So the finished movie “Movie” depends on the "edit" being done, and the "edit" depends on the "footage", etc.  

## Processes

When you write a workflow and load it into a C4 framework service it is valid as long as there is not a syntax error, but it may not yet be computable if all the dependencies are not yet satisfied.  So we can always start with something that is incomplete, but valid, and then add additional details when more information is known.  This allows one to work in a behavior driven design or BDD style. You may stub a dependency out with some fixed value, but then later replace it with a computed or remote value.

One might iterate on developing some functionality like so:

```c4
### v1 hard coded result
    pi
      3.1415

### v2 functional result
    pi -> perl -Mbignum=bpi -wle
      print bpi(200)
```

This example calls out to perl to compute a bignum of pi to 200 digits.  C4 supports the idea of running code in different environments using traditional tools and languages to make it easer to migrate from or incorporate older tools.  This could also be an application run from the command line.  The results that are returned, however, must be in the C4 language (including yaml, or json).

```c4
# v3 functional result with an argument as a dependency 
    pi -> perl -Mbignum=bpi -wle
        print bpi($length)
      length: 200 # length is now a dependency and is passed as a global into the above perl code.

# v4 now with an array dependency, so there will be an array of results (no loop required, it’s implied)
#    in fact, the loop may be run across multiple application instances like vector processors
pi -> perl -Mbignum=bpi -wle
    print bpi($length)
  length: [5, 10, 50, 100, 200]

# v5 now the dependency as a function
pi -> perl -Mbignum=bpi -wle
    print bpi($length)
  length -> ruby  # this time ruby is the language we call out to
      values = 5.times.collect { |i| i*5 } # = [ 0, 5, 10, 15, …]
```

So in affect instead of functions being written with arguments first and function body following like in many languages, the function body is written out first with named arguments written out after and referred too as "dependencies".   This is to allow for visually cohesive nesting of dependencies.  To use a function later in code you invoke it and then replace the values in any dependencies as necessary. Like so:

```c4
result
  pi
    length: 2
```

In this case if you don't replace length with 2 it would have been the functional array from v5 above, so you don't need a priori knowledge of the arguments for pi if you don't need to change them.

# Examples

- [Production Examples](https://github.com/etcenter/C4/tree/master/docs/examples/production)
- [Basic Database Service](https://github.com/etcenter/C4/blob/master/docs/examples/resource_provider/directoryServiceDefinition.c4.md)

More examples in the works.
