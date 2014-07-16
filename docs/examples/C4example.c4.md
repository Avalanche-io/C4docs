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

In this example `pi` is a process that is run in perl when the value of pi is needed.  Referring to this `pi` in a definition will return pi to 200 digits.  However, you could also modify the definition to include additional dependencies or values.  For example :

    pi
      length: 42

Here `pi` will return pi to 42 digits since the length symbol has been redefined to be 42 (but none of the rest of the pi definition has been redefined).

Here we define a user entity.

    joshua
      Entity user
        full name: Joshua Kolden
        email: joshua@studiopyxis.com
        rights
          admin
            Batman Begins
            Studio Pyxis

Symbols can be any valid utf8 string not including control characters, quotes, brackets and (':','=').  In some cases they may have to be quoted to avoid ambiguity.

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
          Batman Begins assets active
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

Here we create the Studio Pyxis entity.  Note that Studio Pyxis only means "Studio Pyxis" within this file / domain.  Within the global domain "com.studiopyxis.c4" and it's aliases would be _registered_ and have certified signing certificates from a certificate authority for validation.

    Studio Pyxis
      Entity company
        production_db
          Resource database
            domain: db.secure.studiopyxis.com
              # in this case 'credentials' is resolved by a process that
              # is run within the given resource and computes a json object.
              # However, this code will only run if this domain accepts
              # the signing identity of this file:
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

Above the `production_db` resources is instructed to call a 'Process' to obtain it's credentials. Processes are declared either with the 'Process' keyword or with a "dash rocket" '->'. Immediately after the '->' (or 'Process') is the language in which the process should be interpreted and any evocation arguments.  In this case bash is called immediately because the '!' makes the directive imperative so the environment variables will be pulled from the local shell as the script is parsed.

# Signatures

C4 files can be signed, and services must authenticate signatures before executing code.  Signature blocks start with `Sign` and end with "Signed [entity alias]", by default a file will be signed from the beginning of the file to the signature line if a `Signed` directive is not matched to a `Sign` block.  If `Sign` is specified then only the block between `Sign` and `Signed [identity alias]` are signed.

Signature blocks can be nested within other signature blocks, or any other type of block but must both be in the same block.  Also the contents of any `Include` files that are in the block are signed as well.

Markdown and comments are included in the signature, but formatting is not.  In other words the 'meaning' of the file is signed, not the layout or 'look'. Incidental changes to a file such as superfluous spaces will not change the signature.

    Signed Joshua Kolden <joshua@studiopyxis.com>
    Signed Studio Pyxis
    
Note decelerations can exist after the signature lines of a file but nothing after the `Signed` line is validated with that signature, so it won't run anywhere that signature is required.


