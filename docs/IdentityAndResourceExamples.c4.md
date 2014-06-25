    Sign
    Sign

`Sign` at the beginning of the file is optional.  If `Signed [identity alias]` is supplied in the file then the file (up to the signature line) is assumed to be signed.  If `Sign` is specified then only the block between `Sign` and `Signed [identity alias]` are signed.  It is an error to specify `Sign` and not provide a `Signed[identity alias]` at some later point in the file. 

Signatures can be nested, markdown / comments are included in the signature, formatting is not.  In other words the 'meaning' of the file is signed, not the actual text of the file.

# C4 Identity and Resource Examples
This is an example of a c4 language file that creates entities, and associates
resources. Note signatures are applied to included files as well.
    
    Include io.cccc.lang.Entity     1.0
    Include io.cccc.lang.Resource   1.0
    Include io.cccc.lang.Process    1.0

Also note that the fully qualified reverse DNS will force validation of signatures to that domain, whereas the version without reverse DNS will not force validation (i.e. `Include Entity     1.0`).

Variables are assigned either in a json format...

    joshua = {Entity {type:"user", {"full name":"Joshua Kolden", {"Batman":{grant:admin}, "StudioPyxis":{grant:admin}}

... OR in a nested format.  Equals ("=") and brackets ("{","}") are optional.  Quotes are usually optional, except in ambiguous cases.

    joshua
      Entity user
        full name: Joshua Kolden
        email: joshua@studiopyxis.com
        Batman
          grant: admin
        StudioPyxis
          grant: admin

Variables can be capitalized or lower case.  And variables become universally accessible within the current scope (which is synonyms with 'domain').  In the following we create a project entity "Batman" and associate various resources.  This way in the future when we associate processes to assets they automatically know what resources to use.

    Batman
      Entity project
        db = StudioPyxis.production_db
        assets
          Resource storage stack
            archive
              Resource storage
                domain: archive.project.com
                type: swift
                access: archive
                cost: $0.01 / GB / Month
            active
              Resource storage
                domain: strage.project.com
                type: swift
                access: active
                cost: $0.13 / GB / Month
        render
          Resource compute stack
            domain: private21.rackspace.com
            type: nova
              m1.large
                Resource compute
                  cost: $0.25 / hour
              c2.xlarge
                performance: 24aiu
                Resource compute
                  cost: $0.50 / hour

Here we create the StudioPyxis entity.  Note that StudioPyxis only means "StudioPyxis" within this domain.  Within the global domain "com.studiopyxis.c4" and it's alias will be _registered_ and have certified signing certificates from certificate authorities for validation.

    StudioPyxis
      Entity company
        Batman
          grant: all
        production_db
          Resource db
            domain: db.secure.studiopyxis.com
            credentials - ruby
                { username: ENV["StudioPyxisUsername"], password: ENV["StudioPyxisPassword"]}

Above the `production_db` resources is instructed to call a 'Process' to obtain it's credentials. Processes are declared either with the 'Process' keyword or a dash '-' (these are synonymous). Immediately after the '-' (or 'Process') keyword is the language in which the process should run.  In this case ruby is called and returns a hash that is filled with environment variables. Data / inline processes like this one should return an object that responds to 'to_json()', or is a json a string. 

It's important to note that the environment variables are filled in from the execution environment which is not necessarily the current environment.  For this reason one should be cautious in using environment variables.  I use it here to simplify the example.
            
    Signed Joshua Kolden <joshua@studiopyxis.com>
    Signed com.studiopyxis
    
Note nothing after the `Signed` line is validated with the signature.
