# C4 General Data Processing Example
Here we define a set of commands to ingest a .mov file (defined in the "ExtractFrames.c4" file), extract the frames, apply some effect on the frames, and then bundle the frames into a mov file that is copied to "processedMovieFile.mov" locally.

    Include C4 1.0

    processed movie file  # the Symbol's name is: "processed movie file"
      Process copy
        targets
          file://processedMovieFile.mov
        MakeH264: -> bash
            makeH264command $effect
          effect: -> ruby # this is a dependency of MakeH264
              #ruby code to do some work with $frame_list
            frame_list
              ExtractFrames
                # ExtractFrames.c4.md is automatically loaded and evoked here.

    processed movie file!

'!' makes a definition imperative, and causes the dependencies to be evaluated.  At some point in the future the work will be done.

# C4 json Example
This is the same thing but as a json object.

```json
{
  Include: "file://1.0/C4.c4",
  "processed movie file": {
    comments: "# the Symbol's name is: \"processed movie file\"",
    Processes: [
      {
        invoke: {"$ref": "Process.copy"},
        targets: [ "file://processedMovieFile.mov" ],
        MakeH264: {
          Processes: [
            {
              invoke: {"$ref": "Process.bash"},
              body: "# do some bash script to make the movie file using as input the\n# output of \"effect\"",
              effect: {
                Processes: [
                  {
                    comments: "# this is a dependency of MakeH264",
                    invoke: {"$ref": "Process.ruby"}
                    body: "#ruby code to do some work with the output of \"ExtractFrames\"",
                    ExtractFrames: {"$ref": "file://ExtractFrames.c4"}
                  }
                ]
              }
            }
          ]
        }
      }
    ]
  },
  execute: [
    {"$ref": "processed movie file"}
  ]
}
```


