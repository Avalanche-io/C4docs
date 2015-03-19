# C4 - Interoperability Tools and Services 

This quick start guide introduces the key concepts of C4, for enabling interoperability between various types of production software and web service APIs. It includes examples of how to publish web service APIs in a way that can be consumed by other C4 compliant tools and services, how to 'wrap' existing software to make it C4 compliant.

More information can be found at the ETC - [C4 GitHub project](https://github.com/etcenter/C4).

## C4 Interoperability Key Concepts

In C4 interoperability is the idea of connecting tools, organizations, or phase of production together.  For example connecting the output of one software tool to the input of another, or connecting a production computer to a studio service, or even connecting people and companies to each other. These connections describe workflows that in turn can be used to automate aspects of production, improving reliability and productivity.

Interoperability is a significant value regardless of the cloud (meaning scalable, distributed production resources). However it becomes essential for enabling cloud based production. This is due to the quickly overwhelming complexity of asynchronous communication, data duplication between tools and organizations, and unreliable/inconstant communication between all the participants in a production.

C4 makes this interoperability possible via an innovative Unified Asset Address Space, and also makes publishing and consuming APIs easer with the C4 domain language.

### Asset IDs - The Unified Asset Address Space

C4 defines any digital Asset that is contained within a single file to be an **Asset**.  Collections of Assets such as a sequence of frames, or a folder of files are **Groups**.

Asset have a unique ID that is a cryptographic hash of the Asset itself.  What this means is that a single unique 512 bit number is computed from all the bites that make up the Asset file.

Asset IDs have some interesting and critical properties.

- An Asset ID can be computed from the Asset itself (no registry)
- A given file will *always* have the same ID regardless of who compute it
- Files with even a single bit different will have completely different IDs
- It is effectively impossible to find two different files with the same ID

These properties are important because they enable the Unified Asset Address Space design at the heart of C4s Asset system.  You might think of the Unified Asset Address Space as a nearly infinite wall of post office boxes, each one represented by a single Asset ID, and each containing a single Asset.  Everyone shares the same wall of PO boxes.

What this means is that if there are two companies working independently from each other and they have the same Asset ID they would both look inside the same PO box.  If they both had the same Asset then they would agree on what it's PO box address has.

So now instead of company A and company B having different paths to a file, different naming conventions, not to mention completely different and isolated infrastructures entirely they both share the Unified Asset Address Space.  In C4 this happens automatically even if the companies never talked to each other.

We often think of saving over a file as a revision of a single file, but from the C4 Asset ID point of view these multiple versions of a file represent completely distinct Assets. Links can be made between assets to establish relationships like version.

### C4 Domain Language for Publishing APIs

The C4 domain language can be used to document an API in a way that can act as both documentation for the human reader, and interface specification software.

Helpful features of the C4 DSL (domain specific language):

- Markdown base format that encourages inline documentation and is easily read.
- YAML style code that are much more readable then XML and the like.
- Declarative dependency graph structure that withstands asynchronicity.
- Code compiles easily to YAML and JSON.
- Supports eventual consistency based systems, and massive parallelism.
- Schemas extend simple data types with validation constraints.
- Extends JSON (a data only format) with function definition in any language.

---

## Publishing a Web API

Given a simple REST based web service with a JSON API.  The service provider, let's say `acmefilmservice.com`, would publish one or more C4 format documents rooted at `https://cccc.acmefilmservice.com`.  This C4 file might be self contained or include other files.

The C4 URL prefix above is a specific requirement defined to support automated service discovery. Meaning that tools looking at a domain for a C4 interface will always check `https://cccc.*` first for the interface definition.

See acmefilmservice.c4 for an example of what would be returned by accessing `https://cccc.acmefilmservice.com`.

### Defining the API

Here's an example of a simple C4 definition for a login API.

    AcmeUserLogin @ POST api.acmefilmservice.com/v1/login            # 1
      < session_key                                                  # 2
        - length: 32                                                 # 3
      < error: Incorrect username or password                        # 4
      < error: Invalid inputs                                        # 5
      < error: Internal server error                                 # 6
      username <                                                     # 7
        - length: [4-50]                                             # 8
          error: "Usernames must be 4 to 50 characters"              # 9
        - format                                                     # 10
          without: /\s/                                              # 11
          error: "Usernames may not contain any spaces."             # 12
      password <                                                     # 13
        - length: [1-50]                                             # 14

It consists of 3 basic parts.  

- Line 1: Name access method url (https assumed).
- Line 2-6: Possible return values.
- Line 7-14: Inputs and validations.

This API endpoint expects a JSON object containing a `username` value and a `password` value.  It may return a JSON object with a single member `session_key` that will contain a String that is 32 bytes long (default type is String).  It may also return any one of 3 other values which would be a JSON object with a single member `error` defined as one of the string listed.

Notice also that inputs and outputs may have, but are not required to have validations. `username` is validated for length and formatting, while the password is only validated for length.

Because a C4 file is markdown a more idiomatic way to present the Acme Login service would be to declare it and document it at the same time.  See the acmefilmservice.c4 file for an example of how this is done.

### Symbols

Note the special symbols and their meaning.

    `<`   - Output
    `>`   - Dependency 
    `-`   - List item, or array element.
    `->`  - Function definition 

For example the output of this API call will be: `{session_key:"some key value"}` if the login is successful where "some key value" will always be a UTF-8 String that is exactly 32 characters long. UTF-8 String are the default representation and type so no type or representation is given.

---

## Wrapping an Application
Here is a simple example of wrapping a command line application `exiftool` to support a C4 interface.  The C4 language 

One important concept to keep in mind is that the C4 DSL is itself a superset of meta data that adds functionality and dependencies. Will illustrate that by wrapping the `exiftool` (meta data extractor) and show how the exiftool output flows naturally up the C4 dependency graph.

First lets conceptualize the problem. Here's how we might normally invoke `exiftool` on the command line manually (long output truncated).

```bash
> exiftool ./testImage.jpg
ExifTool Version Number         : 9.72
File Name                       : Image1.jpg
Directory                       : /Users/joshua/ws/C4Framework/testData/Images
File Size                       : 6.9 MB
File Modification Date/Time     : 2014:12:15 20:06:01-06:00
File Access Date/Time           : 2015:01:20 06:28:25-06:00
File Inode Change Date/Time     : 2014:12:15 20:06:01-06:00
File Permissions                : rw-r--r--
File Type                       : JPEG
MIME Type                       : image/jpeg
```
  [snip]
```bash
Lens ID                         : AF-S DX VR Zoom-Nikkor 18-200mm f/3.5-5.6G IF-ED or AF-S DX VR Zoom-Nikkor 18-200mm f/3.5-5.6G IF-ED [II]
Light Value                     : 14.6
```

`exiftool` can extract JSON data via the `-j` instantly making it C4 compatible.  So let's try that...

```bash
> exiftool -j testImage.jpg
[{
  "SourceFile": "/Users/joshua/ws/pbz/popboardz-osx/Testing/testData/Images/Image3.jpg",
```
  [snip]
```bash
  "LightValue": 14.6
}]
```

Great except for our demo purposes I don't want an array.  So I'll use [jq](https://github.com/stedolan/jq) to extract the first element in the array...

```bash
> exiftool -j testImage.jpg | jq '.[0]'
{
  "SourceFile": "/Users/joshua/ws/pbz/popboardz-osx/Testing/testData/Images/Image3.jpg",
```
  [snip]
```bash
  "LightValue": 14.6
}
```

Perfect.  So now let's wrap this with C4:

    exiftool -> bash
        exiftool -j testImage.jpg | jq '.[0]'

Done, that's it. In this case the output is implied to be the result of the command.  Of course you could (and should) document what exiftool will output in terms of format, but it's not strictly required.  

The above two lines of C4 code run the exiftool, and ship the JSON output of `exiftool`/`jq` up the dependency graph. 

Now let's make the filename a dependency.

    exiftool -> bash
        exiftool -j "${path}" | jq `.[0]`
      path <
        - length: [1-4096]                                
          error: "Path cannot be longer than 4096 characters." 

And now let's not output the entire JSON object just the parts of it we want.

    lensInfo -> bash
        exiftool -j "${path}" | jq `.[0]`
      <
        .Lens
        .ApproximateFocusDistance
        .FocalLengthIn35mmFormat
        .FOV
        .FocalLength35efl
        .LensID
      < error: /File not found:.*/
      < error: .error
      path <
        - length: [1-4096]                                
          error: "Path cannot be longer than 4096 characters." 

Now this new node "lensInfo" delivers a hash with 6 specific values `{"Lense":...`.  not the entire result of the exiftool.  Also note the additional error declarations, although you'd no doubt what to cover all the possible errors, nut this demonstrates how to use a value from the returned JSON object.
