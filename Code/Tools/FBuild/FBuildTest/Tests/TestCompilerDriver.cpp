// TestCompilerDriver.cpp
//------------------------------------------------------------------------------
#if defined( __WINDOWS__ )

// Includes
//------------------------------------------------------------------------------
    #include "FBuildTest.h"

// FBuildCore
    #include "Tools/FBuild/FBuildCore/ExeDrivers/Compiler/CompilerDriver_CL.h"
    #include "Tools/FBuild/FBuildCore/ExeDrivers/Compiler/CompilerDriverBase.h"
    #include "Tools/FBuild/FBuildCore/Helpers/Args.h"

// Core
    #include "Core/Strings/AStackString.h"

// Constants - remote source root containing spaces (exercises quoting)
//------------------------------------------------------------------------------
static const char * const gRemoteSourceRoot = "C:\\Users\\Jose M. Nieves\\Documents\\Engine\\Source";

// Expected args for a relative include "Core" expanded against the remote
// source root (plus the one trailing space added by AddDelimiter)
static const char * const gExpectedCoreIncludeArgs = "/I\"C:\\Users\\Jose M. Nieves\\Documents\\Engine\\Source\\Core\" ";

//------------------------------------------------------------------------------
TEST_GROUP( TestCompilerDriver, FBuildTest )
{
public:
    // Helpers
    void CheckInclude( const char * token,
                       bool isLocal,
                       bool expectHandled,
                       const AString & expectedArgs ) const;
};

//------------------------------------------------------------------------------
void TestCompilerDriver::CheckInclude( const char * token,
                                       bool isLocal,
                                       bool expectHandled,
                                       const AString & expectedArgs ) const
{
    CompilerDriver_CL cl( false );
    cl.Init( nullptr, AStackString( gRemoteSourceRoot ) );

    // ProcessArg_CompilePreprocessed is protected on CompilerDriver_CL but
    // public on the base class, so invoke it through a base pointer
    CompilerDriverBase * base = &cl;
    size_t index = 0;
    AStackString nextToken;
    Args args;
    const bool handled = base->ProcessArg_CompilePreprocessed( AStackString( token ), index, nextToken, isLocal, args );

    TEST_ASSERT( handled == expectHandled );
    TEST_ASSERT( args.GetRawArgs() == expectedArgs );
}

//------------------------------------------------------------------------------
TEST_CASE( TestCompilerDriver, RegressionTest_RemoteIncludeWithSpaceInPath )
{
    // Relative include paths are expanded against the remote source root and
    // quoted so that spaces in the path cannot split the argument

    // Bare relative include
    CheckInclude( "/ICore", false, true, AStackString( gExpectedCoreIncludeArgs ) );

    // Quoted relative include: quotes are stripped before expansion
    CheckInclude( "/I\"Core\"", false, true, AStackString( gExpectedCoreIncludeArgs ) );

    // Quoted include with a trailing backslash: the backslash is doubled so it
    // cannot escape the closing quote when parsed by cl.exe
    CheckInclude( "/I\"Core\\\"", false, true,
                  AStackString( "/I\"C:\\Users\\Jose M. Nieves\\Documents\\Engine\\Source\\Core\\\\\" " ) );

    // Empty quoted include: the expanded path ends with the added separator,
    // whose trailing backslash is doubled before the closing quote
    CheckInclude( "/I\"\"", false, true,
                  AStackString( "/I\"C:\\Users\\Jose M. Nieves\\Documents\\Engine\\Source\\\\\" " ) );
}

//------------------------------------------------------------------------------
TEST_CASE( TestCompilerDriver, RemoteIncludeFullPathUntouched )
{
    // Full paths (quoted or not) are left alone by the preprocessing
    CheckInclude( "/I\"C:\\Full\\Path\"", false, false, AString::GetEmpty() );
    CheckInclude( "/IC:\\Full\\Path", false, false, AString::GetEmpty() );
}

//------------------------------------------------------------------------------
TEST_CASE( TestCompilerDriver, LocalIncludeNotExpanded )
{
    // Relative includes are only expanded for remote jobs
    CheckInclude( "/ICore", true, false, AString::GetEmpty() );
}

//------------------------------------------------------------------------------
#endif
