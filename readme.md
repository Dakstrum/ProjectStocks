
# Notice

This repository contains the original version of the game written in C. It is less feature-rich than the rust version and is no longer supported.

# Project stocks

Project stocks is a whacky and simple trading game. In it, you can buy and sell stocks, invest in portfolio's and predict market fluctuactions. Do you have what it takes to become a millionaire? How about a zintillionaire? 

# Details

## For players

Project stocks is a highly configurable, lean and fast machine. Want to introduce new companies, products or eavesdrops? Well, as long as you know a bit of JSON (it's easy if you don't!) you can!

On the backend, Project stocks implements an unrealistic, but reasonable, stock simulation algorithm.

## For developers

### License

We believe games are often cherished by players long after the developer has moved onto bigger and better software.

We therefore have made Project stocks licensed under the permissive MIT license. Anyone is capable of keeping this software going with new features, bugfixes and support for platforms that may exist in the future.

Our music and artwork may not be distributed or adapted, either commercial or otherwise.

### Contribution

Want to see a change make it into our official builds? Create a pull request and we'll review your code!

Please note, we build against Windows 10 and Fedora 30. Your changes will need to be compilable and runnable on those two platforms.

### Code and dependencies

Project stocks is written in C11 and was built to be multiplatform. Want to run it on FreeBSD, a raspberry pi or mac osx? Well, as long as you have the following dependencies, you can!

- Allegro 5.2
    - Audio Addon
    - Image Addon
    - Video Addon
    - Font Addon
    - TTF Addon
- json-c
- SQLite3

### Platforms

*Binaries built on*
- Windows 10
- Fedora 30

*Linux binaries tested against*
- Fedora 30
- OpenSUSE Tumbleweed as of 9/8/2019
- Ubuntu 18.04 LTS

*Unsupported platforms that are built and tested*
- MacOS High Sierra
- FreeBSD 12.0

Check out our wiki for helpful documentation on building project stocks.
