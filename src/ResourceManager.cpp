/*!
 * \file   ResourceManager.cpp
 * \date   12 April 2011
 * \author StefanP.MUC
 * \brief  This class handles all the resources (pathes, files) needed by the
 *         sound and graphics facilities.
 */

#include <OgreConfigFile.h>
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include <CoreFoundation/CoreFoundation.h>
#endif

#include "ResourceManager.h"

template<> ResourceManager* Ogre::Singleton<ResourceManager>::ms_Singleton = 0;

const std::string ResourceManager::PLUGINSCFG = "plugins.cfg";
const std::string ResourceManager::RESOURCECFG = "resources.cfg";
const std::string ResourceManager::MUSICSUBPATH = "music/";
const std::string ResourceManager::SOUNDSUBPATH = "sounds/";

/* TODO: insert some general easy-access functions that do all the
 * OgreResourceMenager calls (e.g. returning all sound and music names/files)
 */

/*! \brief Initializes all paths and reads the ogre config file.
 *
 *  Provide a nice cross platform solution for locating the configuration
 *  files. On windows files are searched for in the current working
 *  directory, on OS X however you must provide the full path, the helper
 *  function macBundlePath does this for us.
 */
ResourceManager::ResourceManager() :
        resourcePath(""),
        homePath("./"),
        pluginsPath(""),
        musicPath(""),
        soundPath(""),
        macBundlePath("")
{
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
    char applePath[1024];
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    assert(mainBundle);

    CFURLRef mainBundleURL = CFBundleCopyBundleURL(mainBundle);
    assert(mainBundleURL);

    CFStringRef cfStringRef = CFURLCopyFileSystemPath( mainBundleURL, kCFURLPOSIXPathStyle);
    assert(cfStringRef);

    CFStringGetCString(cfStringRef, applePath, 1024, kCFStringEncodingASCII);

    CFRelease(mainBundleURL);
    CFRelease(cfStringRef);

    macBundlePath = std::string(applePath + "/");

    resourcePath = macBundlePath + "Contents/Resources/";
#elif OGRE_PLATFORM == OGRE_PLATFORM_LINUX
    // getenv return value should not be touched/freed.
    char* path = std::getenv("OPENDUNGEONS_DATA_PATH");
    if(path != 0)
    {
        resourcePath = path;
        if (*resourcePath.end() != '/')
        {
            resourcePath.append("/");
        }
    }

    /* If variable is not set, assume we are in a build dir and
     * use the current dir for config files.
     */
    char* useHomeDir = std::getenv("OPENDUNGEONS_DATA_PATH");
    if (useHomeDir != 0)
    {
        //On linux and similar, use home dir
        //http://linux.die.net/man/3/getpwuid
        char* path = std::getenv("HOME");
        homePath = (path != 0)
                ? path
                : ".";

        homePath.append("/.OpenDungeons");

        //Create directory if it doesn't exist
        struct stat statbuf;
        if (stat(homePath.c_str(), &statbuf) != 0)
        {
            mkdir(homePath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        }

        homePath.append("/");
    }
#endif

#ifndef OGRE_STATIC_LIB
    pluginsPath = resourcePath + PLUGINSCFG;
#endif

    soundPath = resourcePath + ResourceManager::SOUNDSUBPATH;
    musicPath = resourcePath + ResourceManager::MUSICSUBPATH;
}

/*! \brief Returns access to the singleton instance
 */
ResourceManager& ResourceManager::getSingleton()
{
    assert(ms_Singleton);
    return(*ms_Singleton);
}

/*! \brief Returns access to the pointer to the singleton instance
 */
ResourceManager* ResourceManager::getSingletonPtr()
{
    return ms_Singleton;
}

void ResourceManager::setupResources()
{
    Ogre::ConfigFile cf;
    cf.load(resourcePath + RESOURCECFG);

    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName = "";
    Ogre::String typeName = "";
    Ogre::String archName = "";
    while(seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap* settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = resourcePath + i->second;
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
            // OS X does not set the working directory relative to the app,
            // In order to make things portable on OS X we need to provide
            // the loading with it's own bundle path location.
            // Unlike windows you can not rely on the curent working directory
            // for locating your configuration files and resources.

            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                    Ogre::String(std::string(macBundlePath) + archName), typeName, secName, true);
#else
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                    archName, typeName, secName, true);
#endif
        }
    }
}
