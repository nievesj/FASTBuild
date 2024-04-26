# FASTBuild For Unreal Engine v1.13

This version of FASTBuild has been modified to work with UrealEngine 4.27 and 5.3 and it follows the amazing work done by [NineWorldsStudios](https://github.com/NineWorldsStudios/FASTBuild).

## How do I compile this thing?!

Easy! The path with least friction is to use Visual Studio 2022, any version works, I used Community Edition.
* Locate file [VS2022.bff](External%2FSDK%2FVisualStudio%2FVS2022.bff)
* In there look for  `.VS2022_Version         = '14.38.33130'`
* This number `14.38.33130` is important as it needs to map to your installed SDK version, for example C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130
* It's recommended to use the same version as the .bff, but if your number is higher start from there by modifying the .bff file.

To build FASTBuild a version of it is required so head to the [downloads section](https://www.fastbuild.org/docs/download.html) and grab the latest one.

* For simplicity I moved the downloaded FBuild.exe to `FASTBuild\Code` where [fbuild.bff](Code%2Ffbuild.bff) is located.
* Open a windows terminal on path `FASTBuild\Code` then type `.\FBuild.exe`
* That should start the build process and it should be done in a few minutes.
* If you have any errors it's more than likely they are related to the installed SDK version or any missing libraries.

## Unreal Engine Setup

UE needs a few minor changes to be able to work with FastBuild and the Dashboard, you can browse this [changelist](https://github.com/nievesj/UnrealEngine/commit/1ff1e3113fe19e9e531276698dff968e4bab1ba9#diff-94843f776dd2b8dedfa47723cf154f0bd9dab3dc94cc464fceb7e1a6ab735315) to see the modifications. The most important one is in [FastBuildUtilities.cpp](https://github.com/nievesj/UnrealEngine/commit/1ff1e3113fe19e9e531276698dff968e4bab1ba9#diff-94843f776dd2b8dedfa47723cf154f0bd9dab3dc94cc464fceb7e1a6ab735315) as it adds a missing dll required to build shaders.

## Sample BuildConfiguration.xml

The final step is activating FASTBuild by modifying the BuildConfiguration. This is an example of the xml I use.

```
<?xml version="1.0" encoding="utf-8" ?>
<Configuration xmlns="https://www.unrealengine.com/BuildConfiguration">
    <bAllowFASTBuild>true</bAllowFASTBuild>

    <BuildConfiguration>
        <MaxParallelActions><THE MAX NUMBER OF LOCAL LOGICAL CORES YOU WANT TO USE></MaxParallelActions>
    </BuildConfiguration>

    <FASTBuild>
        <bEnableCaching>true</bEnableCaching>
        <CacheMode>ReadWrite</CacheMode>
        <FBuildBrokeragePath>\\NETWORK_PATH_TO\FASTBuildBrokerage</FBuildBrokeragePath>
        <FBuildCachePath>\\NETWORK_PATH_TO\FASTBuildCache</FBuildCachePath>
    </FASTBuild>
</Configuration>
```

## FASTBuild Dashboard
https://github.com/nievesj/FASTBuild-Dashboard
This is a companion app that shows a visual representation of the build progress. If you're familiar with IncrediBuild, it does roughly the same thing.

------------------

# FASTBuild

FASTBuild is a build system for Windows, OSX and Linux, supporting distributed compilation and object caching. It is used by many game developers, from small independent teams to some of the largest studios in the world.

FASTBuild's focus is on fast compile times in both full build and local iteration scenarios.

A large variety of compilers and target architectures are supported. More details, including hosted documentation and Binary downloads can
be found here: http://fastbuild.org

## Branch policy

**Patches will only be accepted into the "dev" branch.**

| Branch | Purpose |
| :----- | :----- |
| main   | Stable branch containing snapshot of latest release |
| dev    | Development branch for integration of pull requests |

## Contribution Guidelines

Improvements and bug fixes are gladly accepted. FASTBuild has been improved immensely by the contributions of many users. To help facilitate ease of integration, please:

**Constrain pull requests to individual changes where possible** - Simple changes are more easily integrated and tested. Pull requests with many changes, where there are issues with one change in particular, will delay the integration of all changes. Pull requests that change or add large amounts of functionality are harder to test and will take much longer to integrate, increasing the likelyhood of conflicts.

**Avoid refactoring and formatting changes mixed with functional changes** - Behavior altering changes (fixes, enhancements and new features) mixed with style changes or refactoring cleanup make changes more difficult to integrate. Please consider splitting these changes so they can more easily be individually integrated.

**Update and extend tests if appropriate** - There are a large set of unit and functional tests. Please update or add new tests if appropriate.

**Update documentation if appropriate** - For changes in behaviour, or addition of new features, please update the documentation.

**Adhere to the coding style** - Please keep variable/function naming, whitespace style and indentation (4 space tabs) consistent. Consistency helps keep the code maintainable.
