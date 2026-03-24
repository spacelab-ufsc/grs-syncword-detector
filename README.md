<h1 align="center">
    GRS Syncword Detector
    <br>
</h1>

<h4 align="center">Syncword Detector of the SpaceLab's Ground Station.</h4>

<p align="center">
    <a href="https://github.com/spacelab-ufsc/grs-syncword-detector">
        <img src="https://img.shields.io/badge/status-development-green?style=for-the-badge">
    </a>
    <a href="https://github.com/spacelab-ufsc/grs-syncword-detector/releases">
        <img alt="GitHub commits since latest release (by date)" src="https://img.shields.io/github/commits-since/spacelab-ufsc/grs-syncword-detector/latest?style=for-the-badge">
    </a>
    <a href="https://github.com/spacelab-ufsc/grs-syncword-detector/blob/main/LICENSE">
        <img src="https://img.shields.io/badge/license-GPL3-yellow?style=for-the-badge">
    </a>
</p>

<p align="center">
    <a href="#overview">Overview</a> •
    <a href="#dependencies">Dependencies</a> •
    <a href="#building">Building</a> •
    <a href="#documentation">Documentation</a> •
    <a href="#license">License</a>
</p>

## Overview

GRS Syncword Detector is a C library component of SpaceLab's Ground Station signal processing pipeline. It sits between the demodulator and the decoder, and is responsible for identifying the start of a transmission by detecting a known synchronization word (syncword) within an incoming bit stream.

The component supports both MSB-first and LSB-first bit endianness, and performs tolerance-based matching by counting the number of bits that match the expected syncword pattern, allowing it to handle minor bit errors in the received stream.

## Dependencies

This component has no external dependencies beyond the C standard library.

## Building

TODO

## Documentation

The documentation of this project is generated using the Sphinx tool, and it is available [here](https://spacelab-ufsc.github.io/grs-syncword-detector/).

### Dependencies

* Sphinx
* sphinx-rtd-theme

### Building the Documentation

```
make html
```

## License

This project is licensed under GPLv3 license.
