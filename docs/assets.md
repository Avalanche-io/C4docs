# C4 Asset identification 

You can generate the C4 Asset ID on most unix systems (including OSX) with the following command line.

```bash
shasum -a 256 [FileName] | cut -f1 -d\ | xxd -r -p | base64
```

More examples coming very soon.

##### A note on naming conventions
In production today, it is common for assets to be differentiated using naming conventions.  However, this requires _a priori_ knowledge of the convention, is very fragile, and is inconsistent across domains.  

Often naming conventions are used to impart metadata regarding the asset such project, version, layer name, etc.  This, however, is not a good way to store and access metadata about an asset, and excludes other potentially important metadata.  What one really wants is to have the metadata accessible via query, so that you can find assets based on metadata or find more metadata associated with a given asset.  This implies a database like interface, rather than a folder hierarchy.

In addition, modern [object storage](http://en.wikipedia.org/wiki/Object_storage) technology can make the storage of assets based on a single ID very robust, scalable and performant either for local storage and for cloud services like [Amazon S3](http://aws.amazon.com/s3/).  Of course an object store is not required, C4 Assets IDs can be easily mapped to a regular file paths on disk.
