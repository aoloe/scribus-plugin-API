# Scribus API for plugins documentation

The HTML version of this documentation will be published as soon as it will be complete enough to be useful.

For now, you'll have to locally gerate the HTML.

## Generating the documentation

`cd` into the `documentation/` directory and run

    qdoc ./config.qdocconf

the documentation files will be created in the `html/` directory.

## Documentating the code

For each class that you want to see documented you have to add

    /*!
        \class ScribusAPIClassName
        \brief Description of the class

        All about this class.
     */

Otherwise no documentation file will be created.

Methods are automatically listed, and you can add specific documentation with

    /*!
        \fn ScribusAPIClassName::method()

        All about this method.

    */

## Creating and modifying the config file

A config file (for c++ code) needs at least:


    outputdir   = html
    headerdirs  = .
    sourcedirs  = .
    exampledirs = .
    imagedirs   = ./images


    sources.fileextensions +=  *.cpp *.qdoc
    headers.fileextensions +=  *.h

## Todo

- Check if it would not be be better to use Doxygen instead.
