// CompilerDriverBase.h
//------------------------------------------------------------------------------
#pragma once

// Includes
//------------------------------------------------------------------------------
// Core
#include "Core/Env/Types.h"
#include "Core/Strings/AString.h"

// Forward Declarations
//------------------------------------------------------------------------------
class Args;
class Job;
class ObjectNode;

// CompilerDriverBase
//------------------------------------------------------------------------------
class CompilerDriverBase
{
public:
    CompilerDriverBase();
    virtual ~CompilerDriverBase();

    void Init( const ObjectNode * objectNode,
               const AString & remoteSourceRoot );

    // Append "path" and always double the trailing backslashes (if any) inside
    // the quotes: a backslash immediately before a closing quote is parsed as an
    // escaped quote by MSVC command-line/response-file processing (cl.exe),
    // corrupting the rest of the args. Used when emitting paths that may be
    // quoted (e.g. /I"...") containing spaces or trailing backslashes.
    static void AppendQuotedMSVCPath( Args & out, const AString & path );

    void SetForceColoredDiagnostics( bool forceColoredDiagnostics ) { m_ForceColoredDiagnostics = forceColoredDiagnostics; }
    void SetUseSourceMapping( const AString & sourceMapping ) { m_SourceMapping = sourceMapping; }
    void SetRelativeBasePath( const AString & relativeBasePath ) { m_RelativeBasePath = relativeBasePath; }
    void SetOverrideSourceFile( const AString & overrideSourceFile ) { m_OverrideSourceFile = overrideSourceFile; }

    // Manipulate args if needed for various compilation modes
    virtual bool ProcessArg_PreprocessorOnly( const AString & token,
                                              size_t & index,
                                              const AString & nextToken,
                                              Args & outFullArgs ) const;
    virtual bool ProcessArg_CompilePreprocessed( const AString & token,
                                                 size_t & index,
                                                 const AString & nextToken,
                                                 bool isLocal,
                                                 Args & outFullArgs ) const;
    virtual bool ProcessArg_Common( const AString & token,
                                    size_t & index,
                                    Args & outFullArgs ) const;

    // Inject build-time substitutions (%1 etc)
    virtual bool ProcessArg_BuildTimeSubstitution( const AString & token,
                                                   size_t & index,
                                                   Args & outFullArgs ) const;

    // Add additional args
    virtual void AddAdditionalArgs_Preprocessor( Args & outFullArgs ) const;
    virtual void AddAdditionalArgs_Common( bool isLocal,
                                           Args & outFullArgs ) const;

    // Locally modify args before passing to remote worker
    virtual bool ProcessArg_PreparePreprocessedForRemote( const AString & token,
                                                          size_t & index,
                                                          const AString & nextToken,
                                                          Args & outFullArgs ) const;
    virtual void AddAdditionalArgs_PreparePreprocessedForRemote( Args & outFullArgs );

protected:
    static bool StripTokenWithArg( const char * tokenToCheckFor,
                                   const AString & token,
                                   size_t & index );
    static bool StripToken( const char * tokenToCheckFor,
                            const AString & token,
                            bool allowStartsWith = false );

    const ObjectNode * m_ObjectNode = nullptr;
    bool m_ForceColoredDiagnostics = false;
    AString m_SourceMapping;
    AString m_RelativeBasePath;
    AString m_OverrideSourceFile;
    AString m_RemoteSourceRoot;
};

//------------------------------------------------------------------------------
