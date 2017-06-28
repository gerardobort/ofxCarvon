# ofxCarvon [WIP]

This library provides a variety of tools for fast-prototryping of camera-based applications.  The library counts with some helpful transformers and debugger tools that helps calibrating cameras, align many together (from multiple types of sources: network device, video files).  It also provides easy views for 180deg, 180deb side-by-side and 360 sphere-dome.
The project, apart of the OpenFrameworks library, will include some extra tools, like an MJPEG python server example for Raspberry, a Three.JS web view equivalent for each ofx view.

## Pipeline Components

### Processors

- Source
    - Device
    - FileVideo
    - FileImage - TODO
    - Network (mjpeg)

- Transformer
    - Standard
    - Debugger fisheye
    - SphereTextiure
    - SphereComposedTextiure

- View
    - Half-sphere (180deg)
    - Sphere (360deg) - TODO
    - SBS - TODO

### Example

- camera-device-180
