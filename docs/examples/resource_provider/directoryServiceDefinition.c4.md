# Directory Service 
This is a services for associating URI endpoints with asset ids.
Say this file is at "org.etccenter.c4"

    Resource org.etcenter.string
      String
        add: -> bash
            echo "INSERT INTO resources asset_id = ${asset}, resource = ${resource}"
          asset: nil
          resource: nil
        get: -> bash
          echo "SELECT FROM resources WHERE asset_id = ${asset}" | psql 
        asset: nil


---

    Include org.etccenter.c4

    studio database
      String
        asset
          Asset id
            target: "/path/to/local/asset"

    studio database!
