# C4 Example

This entire file is valid C4.  Note the use of markdown, for documentation and extended comments with formatting.

C4 is a declarative language, but can invoke external processes that can be implemented in any language.  Results from these external processes must be a valid right side of a symbol definition in C4 (including json).

It can also invoke any defined symbols imperatively using an exclamation "!" after the symbol.

A C4 file represents a network of dependencies that can be invoked to deliver a result once the network of dependencies is complete and an imperative call is made.  Once a C4 file is loaded into a domain the network persists indefinitely, or until it's definitions are removed or replaced. This allows later scripts to build from an existing network or run imperative operations through an established network.

# Includes

These includes specify what components of the modular interface will actually be used in this file. `io.cccc.lang.C4` includes everything as a convenience.  These can be actual c4 files to include, or already instantiated scopes within the environment. 
    
    Include io.cccc.lang.Entity     1.0
    Include io.cccc.lang.Resource   1.0
    Include io.cccc.lang.Process    1.0
    Include com.rackspace.c4        1.0 # RackSpace provided C4 Resources
    
Also note that the fully qualified reverse DNS will force validation of signatures to that domain, whereas the version without reverse DNS will not force validation, so `Include Entity     1.0` would not be validated for signatures.

# Symbols

Symbols are either a concrete value such as "pi: 3.1415", or a functional definition like:
    
    pi: -> perl -Mbignum=bpi -wle
        print bpi($length)
      length: 200

In this example `pi` is a process that is run in perl when the value of pi is needed.  Referring to this `pi` in a definition will return pi to 200 digits.  However, later you could modify the definition to include additional dependencies or redefine existing ones.  For example :

    pi
      length: 42

Here `pi` will return pi to 42 digits since the `length` symbol the pi process uses has been redefined to be 42 (but none of the rest of the pi definition has been redefined).  This is similar to named arguments in function calls in other languages.

When invoking a symbol you are effectively including the definition of that symbol as if it had been written out, and potentially overriding any of the previously defined dependencies and/or adding new ones.  In the `pi` example `pi` is said to be `dependent` on `length`, so `pi` will not run until `length` is resolved.

# Symbols, Domains, and the Dependancy Graph

Symbol dependencies connect to create a graph of dependencies. Each Symbol represents a node in that graph. Each node may be processed in a different domain, or context within a domain.

So for example a process might run on a server, return results that are digitally signed locally with your private key, and 'delivered' to another user via a process running on their laptop that copies the data from the cloud location.  These separate steps can be intuitively written in a single C4 file, but are executed in three different domains to satisfy the dependencies efficiently.

In the following we create a project entity "Batman Begins" and associate various resources.  In this way we can associate processes with assets in the future and they automatically know what resources to use.

    Batman Begins       # A single symbol deceleration (i.e "Batman Begins")
      # db is assigned a database from a resources that is defined elsewhere.
      db: StudioPyxis.production_db
      assets
        archive
          Resource storage  # Define a storage resource
            domain: archive.project.internal
            type: swift
            access: archive
            cost: $0.01 / GB / Month 
        active
          Resource storage
            domain: strage.project.internal
            type: swift
            access: active
            cost: $0.13 / GB / Month
      render
        Resource compute  
          domain: private21.rackspace.com
          type: nova.m1.large
          cost: $0.25 / hour
          performance: 12
          memory: 24GB
        Resource compute
          domain: private21.rackspace.com
          type: nova.c2.xlarge
          cost: $0.50 / hour
          performance: 24
          memory: 8GB

    myMovieFile
      Process copy # copy this asset into both active and archive storage
        targets
          Batman Begins assets archive
          Batman Begins assets active
        sources
          Asset  # define an asset explicit: 
            "/show/shot/assets/movies/myMovie.mov"

    myMovies
      Process move
        targets
          Batman Begins assets active
        sources
          "/show/shot/assets/movies"

# Entities

Entities are users, organizations, agents, and other 'actors' within the C4 environment. Each with specific rights and permissions over assets and resources.

Here we define a user entity.

    joshua
      Entity user
        full name: Joshua Kolden
        email: joshua@studiopyxis.com
        rights
          admin
            Batman Begins
            Studio Pyxis

Here we create the "Studio Pyxis" entity.  Note that Studio Pyxis only means "Studio Pyxis" within this file / domain.  Within the global domain "com.studiopyxis.c4" and it's aliases would be _registered_ and have certified signing certificates from a certificate authority for validation similar to how web domains work.

    Studio Pyxis
      Entity company
        production_db
          Resource database
            domain: db.secure.studiopyxis.com
              # in this case 'credentials' is resolved by a process that
              # is run within the given resource and computes a json object.
              # However, processes only run if the domain accepts
              # the signing identity of the file, in this case:
              # (Joshua Kolden <joshua@studiopyxis.com>)
            credentials: Process ruby  # we return a json object using ruby
                { username: $username,
                  password: $password }
              username: -> bash! # note the '!' 
                $StudioPyxisUsername # get environment variable from the shell
              password: -> bash! 
                $StudioPyxisPassword
        rights
          all
            Batman Begins

# Processes 

Above the `production_db` resources is instructed to call a 'Process' to obtain it's credentials. Processes are declared either with the 'Process' keyword or with a "dash rocket" '->'. Immediately after the '->' (or 'Process') is the language in which the process should be interpreted and any evocation arguments.  In this case bash is called immediately because the '!' makes the directive imperative so the environment variables will be pulled from the local shell as the script is parsed.

# Signatures

C4 files can be signed, and services must authenticate signatures before executing code.  Signature blocks start with `Sign` and end with "Signed [entity alias]", by default a file will be signed from the beginning of the file to the signature line if a `Signed` directive is not matched to a `Sign` block.  If `Sign` is specified then only the block between `Sign` and `Signed [identity alias]` are signed.

Signature blocks can be nested within other signature blocks, or any other type of block but must start and end in the same block.  Also the contents of any `Include` files that are in the block are signed as well.

Markdown and comments are included in the signature, but formatting is not.  In other words the 'meaning' of the file is signed, not the literal text, layout or 'look'. Incidental changes to a file such as superfluous spaces will not change the signature.

    Signed Joshua Kolden <joshua@studiopyxis.com>
    Signed Studio Pyxis
    
Note decelerations can exist after the signature lines of a file but nothing after the `Signed` line is validated with that signature, so it won't run anywhere that signature is required.
