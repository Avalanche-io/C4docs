# Basic Database Service
This is a service for associating URI endpoints with assets.
Say this file is at "org.etccenter.c4"

    Resource org.etcenter.assetdatabase
      Asset Database
        add: -> bash
            echo "INSERT INTO resources asset_id = ${asset}, resource = ${resource}"
          asset: nil
          resource: nil
        get: -> bash
          echo "SELECT FROM resources WHERE asset_id = ${asset}" | psql 
        asset: !!Asset

`!!Asset` is a type deceleration, and means the 'asset' dependency must be given an asset id as input.

---

    Include org.etccenter.c4

    studio database
      Asset Database
        asset: /path/to/local/asset 

    studio database!
