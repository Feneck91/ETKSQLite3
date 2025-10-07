--------------------------------------------------------------------------------------------------------------------------------------
--
-- compile_ws3.lua 1.0 - Stéphane Château
--
-- Script used to generate source and header files from ws3 description.
-- These files are used to make database binding to help to make request and recordset
-- binding.
--
----------------------------------------------------------------------------------------------
-- Script parameters:
--
-- Call lua.exe "compile_ws3.lua" <ws3 file path> <qt/wx/stl> <header file path> <source file path>
--
-- arg[0] - "compile_ws3.lua" is this script to execute
-- arg[1] - <ws3 file path> is the path of the file used to generate database source code
-- arg[2] - <qt / wx / stl> indicate if the generated file must be generated for Qt, STL or for wxWidgets
-- arg[3] - <header file path> is the full path where to generated the header file (.h)
-- arg[4] - <source file path> is the full path where to generated the source file (.cpp)
--
----------------------------------------------------------------------------------------------
-- ws3 file tags:
--
-- $INCLUDE_FILE$   => use to import another file into the generated one
--                     ex : $INCLUDE_FILE${../../anotherfile.txt} -> replaced by the content of the anotherfile.txt
-- $EXPORT_IMPORT$  => use to add export / import macro before class declaration to be able to export / import a
--                     table structure to another library/executable, by default is ''
--                     ex : $EXPORT_IMPORT${MY_LIB_EXPORT_IMPORT}
-- $PREFIX_MEMBERS$ => prefix member to use for all members variables ('m_' by default)
--                     ex : $PREFIX_MEMBERS${m_}
-- $PREFIX_ARGUMENTS_COPY$ => prefix use for function arguments, by default is '_'
--                     ex : $PREFIX_ARGUMENTS_COPY${_}
-- $PREFIX_ARGUMENTS_REFERENCE$ => prefix use for function arguments passed by reference, by default is '_r'
--                     ex : $PREFIX_ARGUMENTS_REFERENCE${_r}
-- $PREFIX_COLUMN$  => Prefix columns names (static fields) ('COLUMN_FIELD_' by default)
--                     ex : $PREFIX_COLUMN${COL_}
-- $PREFIX_TABLE$   => Prefix to use before the table name ('' by default)
--                     ex : $PREFIX_TABLE${PROJECT_}
-- $PREFIX_STRUCT$  => Prefix to use before the structures ('' by default)
--                     ex : $PREFIX_STRUCT${CMyProject}
-- $TYPE_MEMBERS$   => C++ type to use for database type, ex INTEGER could be a int / long / long 64 in C++
--                     ex : $TYPE_MEMBERS${INTEGER,long}    -> INTEGER table field type will be replaced by long C++ type
--                     ex : $TYPE_MEMBERS${SEXE,eSexe}      -> SEXE (ws3 type) is an C++ enum type
-- $PREFIX_TYPE_MEMBERS$  => Prefix to use for a specific type into the struct that contains data. Give the C++ type!
--                     ex : $PREFIX_TYPE_MEMBERS${QDate,qd} => into the structure, all QDate will be written like : QDate  m_qdMyField;
-- $TYPE_ARGS$      => C++ type to use as argument in getter and setter, often the same as the $TYPE_MEMBERS$.
--                     Could be used to define another argument accessor type. In this case the C++ type must accept this argument accessor
--                     type with '=' operator and can be cast to this type.
--                     ex : $TYPE_MEMBERS${BIG_INT,MyClassLongLong}  <-- Define the BIG_INT as a MyClassLongLong class (in struct)
--                     ex : $TYPE_ARGS${BIG_INT,int64}               <-- Define Getter and setter to int64, in this case the 2 followed functions must be defined
--                                                                   void MyClassLongLong::operator=(int64 _param)  <-- Assignment
--                                                                   MyClassLongLong::operator int64() const        <-- Cast operator
-- $TYPE_DATABASE$  => DATABASE type to use to create table. Could be used with with $TYPE_MEMBERS$ to have own type
--                     ex : $TYPE_DATABASE${SEXE,ENUM}      -> SEXE does not exist as SQL type for database, replaced by ENUM (could be integer, VARCHAR or other if binding is correctly done)
-- $PREFIX_REF$     => prefix before the name of the type. It indicate from a type that this type must be passed as reference
--                     ex : $PREFIX_REF${DATETIME,dt} -> DATETIME type will be pased as reference (const wxDateTime &)
-- $PREFIX_COPY$    => prefix before the name of the type. It indicate from a type that this type must be passed as copy
--                     ex : $PREFIX_COPY${INTEGER,i} -> INTEGER type will be pased as copy (long)
-- $REMOVE_BLOC$    => Remove the bloc in both header and source file, it can be used when initializing all preffix arguments
--                     ex : $REMOVE_BLOC${$PREFIX_TABLE${CMyRecord}}
-- $ONLY_SOURCE_FILE$ => Let the bloc in source file only, it can be used to include header file into source file.
--                     ex : $ONLY_SOURCE_FILE${#include "$FILENAME_H$"}
-- $ONLY_HEADER_FILE$ => Let the bloc in headere file only, it can be used to add comments into header file.
--                     ex : $ONLY_HEADER_FILE${// Comments only into header file}
-- $TYPE_BIND$      =>
-- $FILENAME$       => Automatically replaced by the generated file
--                     ex : // Name of the file: $FILENAME$
-- $FILENAME_H$     => Automatically replaced by the header filename
--                     ex : #include $FILENAME_H$
-- $FILENAME_CPP$   => Automatically replaced by the source code filename
--                     ex : // Name of the file: $FILENAME_CPP$
--
--
-- Database type for columns (known by sqlite3):
--                      INTEGER             - same as BIGINT
--                      BIGINT              - 64 bits number
--                      INT                 - 32 bit number (bind on long C++ type)
--                      UNSIGNED_INT        - 32 bit unsigned number (bind on unsigned long C++ type)
--                      SMALLINT            - 16 bits number (bind on short int C++ type)
--                      UNSIGNED_SMALLINT   - 16 bits unsigned number (bind on unsigned short int C++ type)
--                      TINYINT             - 8 bits number (bind on char C++ type)
--                      UNSIGNED_TINYINT    - 8 bits unsigned number (bind on unsigned char C++ type)
--                      VARCHAR
--                      DOUBLE
--                      NUMERIC
--                      BOOLEAN
--                      DATE
--                      TIME
--                      DATETIME
--                      BLOB
--                      ENUM
--                      VARIANT             - No type, use variant binding

-- Options for columns :
--                      PRIMARY_KEY         : column is a primary key
--                      NOT_NULL            : column could not be NULL, cannot used with primary key
--                      NULL                : column could be NULL, cannot used with primary key
--                      AUTOINCREMENT       : auto increment field, used only with number type
--                      UNIQUE              : auto increment field, used only with number type
--                      ON_DELETE_CASCADE   : only use with foreign key, add on delete cascade for this field
--                      ON_UPDATE_CASCADE   : only use with foreign key, add on update cascade for this field
--                      ON_DELETE_SET_NULL  : only use with foreign key, add on delete set null for this field
--                      ON_UPDATE_SET_NULL  : only use with foreign key, add on update set null for this field
--
-- ETK_SQLITE3_COMMENT_TABLE
-- First item of ETK_SQLITE3_DECLARE_TABLE could be :
--                      DECLARE_DATA_IMPLEMENTATION(template_header.h,template_header.cpp)
--                      into the template, can use $REPEAT_VARIABLE$ to make code for each member (example) :
--                      $REPEAT_VARIABLE${{ "$COLUMN_NAME$", luaU_getset<$PREFIX_STRUCT$$TABLE_NAME$, $VARIABLE_TYPE$,&$PREFIX_STRUCT$$TABLE_NAME$::$VARIABLE_NAME$> },

--------------------------------------------------------------------------------------------------------------------------------------

-- Load the sha2.lua, relativly to the script: same path
-- include crypto lib (md5)
local sha2 = dofile((debug.getinfo(1, "S").source:match[[^@?(.*[\/])[^\/]-$]] or "") .. "sha2.lua")

local VERSION = "1"

--[[--------------------------------------------------------------------------
 Display the command line parameters.

 It is the help file, called when the input parameters are incorrect

 Parameters :
-------------
    none

 Return :
---------
    none
----------------------------------------------------------------------------]]
function DisplayCommandLineHelp()
    if (#arg ~= 0) then
        local strCommandLine = arg[0],iIndex

        for iIndex=1,#arg do
            strCommandLine = strCommandLine .. " " .. arg[iIndex]
        end
        print(string.format("Bad parameters when calling lua script:\n\"%s\"\n",strCommandLine))
    end
    print("Call lua \"compile_ws3.lua\" <ws3 file path> <qt/wx/stl> <header file path> <source file path>")
    print("where")
    print("    \"compile_ws3.lua\" is this script to execute")
    print("    <ws3 file path> is the path of the file used to generate database source code")
    print("    <qt / wx / stl> indicate if the generated file must be generated for Qt, STL or for wxWidgets")
    print("    <header file path> is the full path where to generated the header file (.h)")
    print("    <source file path> is the full path where to generated the source file (.cpp)")
end

--[[--------------------------------------------------------------------------
 Read a file content.

 Parameters :
-------------
    _strFilePath : Path of the file to read

 Return :
---------
    1> true / false depending if the function fails or not
    2> The file contents / error message
----------------------------------------------------------------------------]]
function ReadFileContent(_strFilePath)
    local strFileContent = ""
    -- Open the file
    local file,strError = io.open(_strFilePath, "r")
    if (file == nil) then
        -- if the function fails, try with relative path
        file = io.open(GetRelativePath() .. _strFilePath, "r")
    end
    if (file == nil) then
        -- if the function fails, return the error (the first one)
        return false,strError
    end

    strFileContent = file:read("*a")
    file:close()
    return true,strFileContent
end

--[[--------------------------------------------------------------------------
 Read a file content.

 Parameters :
-------------
    _strFilePath : Path of the file to read

 Return :
---------
    Empty string if the file is not found, the file content else
----------------------------------------------------------------------------]]
function GetFileContent(_strFilePath)
    local strFileContent = ""
    -- Open the file
    local file,strError = io.open(_strFilePath, "r")
    if (file == nil) then
        -- if the function fails, try with relative path
        file = io.open(GetRelativePath() .. _strFilePath, "r")
    end
    if (file == nil) then
        -- if the function fails, return the error (the first one)
        return ""
    end

    strFileContent = file:read("*a")
    file:close()
    return strFileContent
end

--[[--------------------------------------------------------------------------
 Write a file content.

 Parameters :
-------------
    _strFilePath       : Path of the file to write
    _strFileContent    : Content of the file to write

 Return :
---------
    1> true / false depending if the function fails or not
    2> The error message if fails, empty string else
----------------------------------------------------------------------------]]
function WriteFileContent(_strFilePath,_strFileContent)
    -- Open the file
    local file,strError = io.open(_strFilePath, "w+")
    if (file == nil) then
        -- if the function fails, return the error
        return false,strError
    end

    -- Else, read the file, line by line
    file:write(_strFileContent)
    file:close()
    return true,""
end

--[[--------------------------------------------------------------------------
 Get the file path, without filename

 Parameters :
-------------
    strFullPathname : Full file pathname

 Return :
---------
    The file path without filename, ended with last path separator if not
    empty. If the fullpathname doesn't contains any path separator, returns
    an empty string.
----------------------------------------------------------------------------]]
function GetFilePath(strFullPathname)
    -- Search first path separator in reverse string order
    local iFindLastSlashBegin = string.find(string.reverse(strFullPathname),"[\\/]",1,false)
    if (iFindLastSlashBegin ~= nil) then
        -- compute full path with last path separator char
        strFullPathname = string.sub(strFullPathname,1,string.len(strFullPathname) - iFindLastSlashBegin + 1)
    else
        strFullPathname = "" -- No path
    end
    return strFullPathname
end

--[[--------------------------------------------------------------------------
 Get the file name, without path

 Parameters :
-------------
    strFullPathname : Full file pathname

 Return :
---------
    The file name without the path
----------------------------------------------------------------------------]]
function GetFileName(strFullPathname)
    -- Search first path separator in reverse string order
    local iFindLastSlashBegin = string.find(string.reverse(strFullPathname),"[\\/]",1,false)
    if (iFindLastSlashBegin ~= nil) then
        -- compute full path with last path separator char
        strFullPathname = string.sub(strFullPathname,string.len(strFullPathname) - iFindLastSlashBegin + 2)
    else
        strFullPathname = strFullPathname -- Same filename as entry
    end
    return strFullPathname
end

--[[--------------------------------------------------------------------------
 Get the file name, without extension

 Parameters :
-------------
    strFullPathname : Full file pathname

 Return :
---------
    1> The file path without extension, same as input if no extension found
    2> The extension if found, empty string else
----------------------------------------------------------------------------]]
function RemoveExtension(strFullPathname)
    -- Search first '.' in reverse string order
    local iFindLastStarBegin,strExtension = string.find(string.reverse(strFullPathname),".",1,true),""
    if (iFindLastStarBegin ~= nil) then
        -- compute full path with last path separator char
        strExtension = string.sub(strFullPathname,string.len(strFullPathname) - iFindLastStarBegin + 1)
        strFullPathname = string.sub(strFullPathname,1,string.len(strFullPathname) - iFindLastStarBegin)
    end
    return strFullPathname,strExtension
end

--[[--------------------------------------------------------------------------
 Split the string into token

 Parameters :
-------------
    strDelimiter : Token delimiter, must be a string of simple char

 Return :
---------
    A Table with all tokens founds
----------------------------------------------------------------------------]]
function string:split(strDelimiter)
    if (strDelimiter == nil) then
        strDelimiter = "%s"
    end
    local tResult,iNbCount,strToken = {},1
    for strToken in string.gmatch(self, "([^"..strDelimiter.."]+)") do
        tResult[iNbCount] = strToken
        iNbCount = iNbCount + 1
    end
    return tResult
end

--[[--------------------------------------------------------------------------
 Replace all occurence of string into string

 It replace all occurence while at least one replacement has been made
 string.gsub works but when same char are replaced, it don't work.
 Example : into string AAAAA replace AA by A

 string.gsub("AAAAA","AA","A)       => AAA
 string.replaceall("AAAAA","AA","A) => A

 Parameters :
-------------
    strPattern      : Pattern to replace
    strReplacement  : Replacement string

 Return :
---------
    The string with all replacement
----------------------------------------------------------------------------]]
function string:replaceall(strPattern,strReplacement)
    local strResult,iNbReplacement

    strResult = self
    repeat
        strResult,iNbReplacement = string.gsub(strResult,strPattern,strReplacement)
    until iNbReplacement == 0
    return strResult
end

--[[--------------------------------------------------------------------------
 Recompute indexes to remove cariage return before and after text bloc

 Increment / decrement indexes depending of cariage return presence

 Parameters :
-------------
    strContentFile  : Text
    iIndexBeginBloc : Begin index
    iIndexEndBloc   : End index
    bInto           : Check if cariage return are into the bloc of out of the bloc
    bOnlyBegin      : To don' remove too much cariage return, make the test only on begin of the bloc

 Return :
---------
    The new computed indexes
----------------------------------------------------------------------------]]
function ComputeIndexes2RemoveBlankLine(strContentFile, iIndexBeginBloc, iIndexEndBloc, bInto, bOnlyBegin)
    local iIndexMoreLess = (bInto and 1 or -1)

    if (string.sub(strContentFile, iIndexBeginBloc, iIndexBeginBloc) == "\n") then
        iIndexBeginBloc = iIndexBeginBloc + iIndexMoreLess
    end
    if (string.sub(strContentFile, iIndexBeginBloc, iIndexBeginBloc) == "\r") then
        iIndexBeginBloc = iIndexBeginBloc + iIndexMoreLess
    end
    if (not bOnlyBegin) then
        -- Remove last \n or \r if exist
        if (string.sub(strContentFile, iIndexEndBloc, iIndexEndBloc) == "\r") then
            iIndexEndBloc = iIndexEndBloc - iIndexMoreLess
        end
        if (string.sub(strContentFile, iIndexEndBloc, iIndexEndBloc) == "\n") then
            iIndexEndBloc = iIndexEndBloc - iIndexMoreLess
        end
    end
    return iIndexBeginBloc, iIndexEndBloc
end

--[[--------------------------------------------------------------------------
 Remove the first bloc found delimited by $BLOC_NAME${this is the bloc}.

 The bloc $TOTO$ is like :
 -------------------------
 the file is begin here
 $TOTO$
 {
 xxxx  \
 xxxx   |
 xxxx   | => this is the bloc to extract
 xxxx   |
 xxxx  /
 }
 and end here
 -------------------------

 #############################################################################
 (bRemoveTagsOnly is false) => if (1) is true return
 The bloc and tags around are removed, this bloc is removed in return (2)

 return (2) is                                      return (3) is
 -------------------------                          -------------------------
 xxxx  \                                            the file is begin here
 xxxx   |                                           and end here
 xxxx   | => this is the bloc to extract            -------------------------
 xxxx   |
 xxxx  /
 -------------------------

 #############################################################################
 (bRemoveTagsOnly is true) => if (1) is true returns : (bloc found)
 -----------------------------------------------------
 The bloc is not removed, only the tag around it are removed

 return (2) is                                      return (3) is
 -------------------------                          -------------------------
                                                    the file is begin here
 xxxx  \                                            xxxx  \
 xxxx   |                                           xxxx   |
 xxxx   | => this is the bloc to extract            xxxx   | => this is the bloc to extract
 xxxx   |                                           xxxx   |
 xxxx  /                                            xxxx  /
                                                    and end here
 -------------------------                          -------------------------

 Parameters :
-------------
    strBlocName     : Bloc name
    strContentFile  : File text content to find into
    bRemoveTagsOnly : Only remove tags
    iStartIndex     : Begin of the search(optionnal)

 Return :
---------
    (1) true / false depending if the function found the entire bloc or not
    (2) Bloc content, if (1) is false return empty string
    (3) Content file without this bloc, if (1) is false it is the same as
        strBlocName parameter
    (4) Begin of bloc index (0 if function returns false as first return parameter)
    (5) End of bloc index (0 if function returns false as first return parameter)
----------------------------------------------------------------------------]]
function RemoveFirstBloc(strBlocName, strContentFile, bRemoveTagsOnly, iStartIndex)
    local strContentBloc, strRetContentFile
    local bFound = false

    if (iStartIndex == nil) then
        iStartIndex = 1
    end

    -- Regex to find begin of bloc
    local strRegexExpression = string.format("$%s$[^{]*{",strBlocName)
    -- iIndexBeginBloc is the first char of the bloc after the tag and opened bracket
    -- iIndexEndBloc is the last char of the bloc after the closed bracket
    local iIndexBeginBloc, iIndexEndBloc

    -- Find it (regex expression)
    local iFindTagStart, iIndexBeginBloc = string.find(strContentFile, strRegexExpression, iStartIndex, false)
    iIndexEndBloc = iIndexBeginBloc

    if (iFindTagStart~=nil) then
        -- Ok, found
        bFound = true

        -- Found corresponding closed bracket
        local iBracket = 0
        -- Find the closed bracket '}' that close the bloc
        repeat
            local iFindBracketStart, iFindBracketStop = string.find(strContentFile,"[^}{]*[}{]",iIndexEndBloc + 1,false)
            if (iFindBracketStart ~= nil) then
                iIndexEndBloc = iFindBracketStop
                if (string.sub(strContentFile,iFindBracketStop,iFindBracketStop) == "}") then
                    if (iBracket == 0) then
                        -- It is the closed bracket of the main bloc
                        break
                    end
                    iBracket = iBracket - 1
                else -- it is '{' char
                    iBracket = iBracket + 1
                end
            end
        until iFindBracketStart == nil
        if (iBracket ~= 0) then
            -- The ended bracket was not found
            return false, "", strContentFile, 0, 0
        end
    else
        --  If not found, return same string
        return false, "", strContentFile, 0, 0
    end

    -- Here, iIndexBeginBloc point on the first { of the bloc
    -- Here, iIndexEndBloc point on the last } of the bloc
    local iIndexBeginBlocToExtract = iIndexBeginBloc + 1 -- without the first {
    local iIndexEndBlocToExtract = iIndexEndBloc - 1     -- without the last }

    -- Get the content bloc
    -- Remove first \n or \r if exist (into the content bloc)
    -- These lines just remove cariage returns if exists to don't have lot of blank lines
    iIndexBeginBlocToExtract, iIndexEndBlocToExtract = ComputeIndexes2RemoveBlankLine(strContentFile, iIndexBeginBlocToExtract, iIndexEndBlocToExtract, true, bRemoveTagsOnly)
    strContentBloc = string.sub(strContentFile,iIndexBeginBlocToExtract,iIndexEndBlocToExtract)

    if (bRemoveTagsOnly) then
        -- Add text before bloc + only bloc into tags + after bloc
        strRetContentFile = string.sub(strContentFile,1,iFindTagStart - 1) ..
                            strContentBloc ..
                            string.sub(strContentFile,iIndexEndBloc + 1) -- To the end of string
    else
        -- Add text before bloc + after bloc
        iFindTagStart, iIndexEndBloc = ComputeIndexes2RemoveBlankLine(strContentFile, iFindTagStart - 1, iIndexEndBloc + 1, false, false)
        strRetContentFile = string.sub(strContentFile, 1, iFindTagStart) .. string.sub(strContentFile, iIndexEndBloc)
        --strRetContentFile = string.sub(strContentFile, 1, iFindTagStart - 1) .. string.sub(strContentFile, iIndexEndBloc + 1)
    end

    return bFound, strContentBloc, strRetContentFile, iFindTagStart, iIndexEndBloc
end

--[[--------------------------------------------------------------------------
 Remove all bloc found delimited by $BLOC_NAME${this is the bloc}.

 see RemoveFirstBloc

 Parameters :
-------------
    strBlocName     : Bloc name
    strContentFile  : File text content to find into
    bRemoveTagsOnly : Only remove tags

 Return :
---------
    Content file without all these blocs, if (1) is false it is the same as
    strBlocName parameter
----------------------------------------------------------------------------]]
function RemoveAllBlocs(strBlocName,strContentFile,bRemoveTagsOnly)
    local bFound,strBlocContent, iBlocIndexBegin, iBlocIndexEnd

    iBlocIndexBegin = 1
    repeat
        bFound,strBlocContent,strContentFile, iBlocIndexBegin, iBlocIndexEnd = RemoveFirstBloc(strBlocName,strContentFile,bRemoveTagsOnly, iBlocIndexBegin)
    until not bFound

    return strContentFile
end

--[[--------------------------------------------------------------------------
 Replace variables into REPEAT_VARIABLES.

 Parameters :
-------------
    strBlocName     : Bloc name
    strContentFile  : File text content to find into
    bRemoveTagsOnly : Only remove tags

 Return :
---------
    Content file without all these blocs, if (1) is false it is the same as
    strBlocName parameter
----------------------------------------------------------------------------]]
function ReplaceRepeatSection(tblReplace,strTemplateFileContent)
    local bFindBloc,strBlocContent, strWithoutBloc, iBlocIndexBegin, iBlocIndexEnd
    local strKey, strValue

    iBlocIndexBegin = 1
    while (iBlocIndexBegin ~= 0) do
        bFindBloc, strBlocContent, strWithoutBloc, iBlocIndexBegin, iBlocIndexEnd = RemoveFirstBloc("REPEAT_VARIABLE",strTemplateFileContent,false, iBlocIndexBegin)
        if (bFindBloc) then
            -- Element is found, replace all items
            for strKey, strValue in pairs(tblReplace) do
                strBlocContent = string.gsub(strBlocContent,"$" .. strKey .. "%$",strValue)
            end
            -- Add
            strTemplateFileContent =    string.sub(strTemplateFileContent,1,iBlocIndexEnd)
                                     .. strBlocContent
                                     .. string.sub(strTemplateFileContent,iBlocIndexEnd + 1)
            iBlocIndexBegin = iBlocIndexEnd + string.len(strBlocContent)
        end
    end
    return strTemplateFileContent
end

--[[--------------------------------------------------------------------------
 Get the directory of a file.

 Use to be able to load file that are in the same directory as the file.

 Parameters :
-------------
    _strPath : Full file path

 Return :
---------
    The path of the file

----------------------------------------------------------------------------]]
local function GetDirName(_strPath)
    return _strPath:match[[^@?(.*[\/])[^\/]-$]]
end

--[[--------------------------------------------------------------------------
 Get the relative path of the script.

 Use to be able to load file that are in the same directory as the file.

 Parameters :
-------------
    None

 Return :
---------
    The relative path of the script

----------------------------------------------------------------------------]]
function GetRelativePath()
    return GetDirName(debug.getinfo(1, "S").source)
end

--[[--------------------------------------------------------------------------
 Remove blank at the left and right of a string.

 Parameters :
-------------
    _strText : Text to trim.

 Return :
---------
    A trim text
----------------------------------------------------------------------------]]
function Trim(_strText)
    return (string.gsub(_strText, "^%s*(.-)%s*$", "%1"))
end


--[[--------------------------------------------------------------------------
 Create a switch / case bloc.

 Implementation of a "switch" statement. This one is based on Luiz Henrique
 de Figueiredo's switch statement presented in a list message dated Dec 8 1998,
 but the object/method relationship has been flipped around to achieve a
 more traditional syntax in actual use. Nil case variables are also handled -
 there's an optional clause specifically for them (something I wanted), or
 they can fallback to the default clause. (easily changed)
 Return values from the case statement functions are also supported.

Here's sample usage:
c = 1
switch(c) : caseof
{
    [1]     = function (x) print(x,"one") end,
    [2]     = function (x) print(x,"two") end,
    [3]     = 12345, -- this is an invalid case stmt
    default = function (x) print(x,"default") end,
    missing = function (x) print(x,"missing") end
}

-- also test the return value
-- sort of like the way C's ternary "?" is often used
-- but perhaps more like LISP's "cond"
--
print("expect to see 468:  ".. 123 +
    switch(2):caseof
    {
        [1] = function(x) return 234 end,
        [2] = function(x) return 345 end
    })

 Parameters :
-------------
    iCase : Case value.
----------------------------------------------------------------------------]]
function switch(iCase)
    local swtbl =
    {
        casevar = iCase,
        caseof = function (self, code)
            local f
            if (self.casevar) then
                f = code[self.casevar] or code.default
            else
                f = code.missing or code.default
            end
            if (f) then
                if type(f)=="function" then
                    return f(self.casevar,self)
                else
                    error("case "..tostring(self.casevar).." not a function")
                end
            end
        end
    }
    return swtbl
end

--[[--------------------------------------------------------------------------
 Return the first index with the given value (or nil if not found).

 Parameters :
-------------
    array : the array where to find value
    value : the value to find

 Return :
---------
    The index of the first element to find, nil if not found.

----------------------------------------------------------------------------]]
function indexOf(array, value)
    for i, v in ipairs(array) do
        if v == value then
            return i
        end
    end
    return nil
end

----------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------
--                                                                                                                  --
--                                                     MAIN                                                         --
--                                                                                                                  --
----------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------
local lReturn = 0   -- No error. It is the error code returns
local strError = "" -- No error for the moment

if (#arg ~= 4) then
    DisplayCommandLineHelp()
    lReturn = -1
else
    local strTemplateHeaderFile,strTemplateSourceFile,strWS3Content,bOk
    local strWS3Hash,strHHash,strCPPHash -- for hash computation
    local strFileNameWS3,strFileNameH,strFileNameCPP = arg[1],arg[3],arg[4]
    local bGenerateH,bGenerateCPP = true,true
    local bFind,strContentType -- General variable, for all needed
    local strExportImport
    local strwxOrQToRSTL = arg[2]

    --------------------------------------------
    -- 1> Read the ws3 input file content
    bOk, strWS3Content =  ReadFileContent(strFileNameWS3)

    if (not bOk) then
        strError = string.format("Error while reading file: %s",strFileNameWS3)
    else
        --------------------------------------------
        -- 2> Read the template header file content
        bOk, strTemplateHeaderFile =  ReadFileContent("ETKSQLite3TableTemplate.h")
        if (not bOk) then
            strError = string.format("Error while reading file: %s","ETKSQLite3TableTemplate.h")
        else
            strTemplateHeaderFile = Trim(strTemplateHeaderFile)
            --------------------------------------------
            -- 3> Read the template source file content
            bOk, strTemplateSourceFile =  ReadFileContent("ETKSQLite3TableTemplate.cpp")
            if (not bOk) then
                strError = string.format("Error while reading file: %s","ETKSQLite3TableTemplate.cpp")
            else
                strTemplateSourceFile = Trim(strTemplateSourceFile)
            end
        end
    end

    --------------------------------------------
    -- Check qt / wx / STL argument
    if (string.len(strError) == 0) then
        if (string.lower(strwxOrQToRSTL) ~= "wx" and string.lower(strwxOrQToRSTL) ~= "qt" and string.lower(strwxOrQToRSTL) ~= "stl") then
            strError = string.format("Parameter 3 (%s) is unknown, use wx (wxWidgets generation), qt (QT generation) or stl (STL generation)!", strwxOrQToRSTL)
        end
    end

    --------------------------------------------
    -- Manage Include files tags
    if (string.len(strError) == 0) then
        local bFindInclude

        repeat
            -- Fin all $INCLUDE_FILE$ tags and replace by the loaded files contents
            local strBlocContent
            -- Record find flag, bloc content but not the new content without the bloc
            bFindInclude,strBlocContent = RemoveFirstBloc("INCLUDE_FILE",strWS3Content,true)

            if (bFindInclude) then
                -- Find one include
                local strIncludeFilePath = GetFilePath(GetDirName(strFileNameWS3)) .. Trim(strBlocContent)

                --------------------------------------------
                -- Read the include file content
                local bOk, strIncludeFileContent =  ReadFileContent(strIncludeFilePath)
                if (not bOk) then
                    strError = string.format("Error while reading included file (see $INCLUDE_FILE$ tag) - %s",strIncludeFileContent)
                else
                    -- Update, remove the bloc and replate it by the contents of the loaded file
                    strIncludeFileContent = strIncludeFileContent:gsub("%%", "%%%%")
                    bFind,strBlocContent,strWS3Content = RemoveFirstBloc("INCLUDE_FILE",
                                                                         string.gsub(strWS3Content,"$INCLUDE_FILE%$",strIncludeFileContent .. "$INCLUDE_FILE$",1),
                                                                         false)
                end
            end
        until (not bFindInclude) or (string.len(strError) ~= 0)
    end
    -- Manage Include files tags
    --------------------------------------------

    -------------------------------------------------------------------
    -- Check Hash of each file to know if they need to be re-generated
    if (string.len(strError) == 0) then
        local strtmpWS3Content = sha2.hash256(strWS3Content .. VERSION) -- If version is changed, re-build all

        strWS3Hash = GetFileContent(RemoveExtension(strFileNameWS3) .. ".hash")
        if (string.len(strWS3Hash) == 194) then
            -- 3 Hash key with \n returns
            -- Get Hash keys
            strHHash    = string.sub(strWS3Hash,66,129)
            strCPPHash  = string.sub(strWS3Hash,131,194)
            strWS3Hash  = string.sub(strWS3Hash,1,64)
            -- Check hash values
            if (strtmpWS3Content ~= strWS3Hash) then
                -- Both header and CPP must be generated, the origin file has been changed
                strWS3Hash = strtmpWS3Content -- Record current hash
            else
                -- The origin file is same, verify the header and the implementation file
                if (strHHash == sha2.hash256(GetFileContent(strFileNameH))) then
                    bGenerateH = false -- Same header, no need to generate it
                end
                if (strCPPHash == sha2.hash256(GetFileContent(strFileNameCPP))) then
                    bGenerateCPP = false -- Same source file, no need to generate it
                end
            end
        else
            -- The hash file doens't exist or is not the good size, so it is invalid
            strWS3Hash = strtmpWS3Content
        end
    end
    -- Check Hash of each file to know if they need to be re-generated
    -------------------------------------------------------------------

    if (not (bGenerateH or bGenerateCPP)) then
        -- No generation to do, all is up to date
        print(string.format("%s is up to date...",strFileNameWS3))
    elseif (string.len(strError) == 0) then
        -- Need a generation : start it
        local strCPPContent,strHContent = "","" -- Generation contents
        local tabTypeMembers,tabTypeArgs,tabTypeMembersRef,tabTypeMembersCopy,tabTypeBind, tabDatabaseMembers, tabPrefixTypeMembers = {},{},{},{},{},{},{}
        local strPrefixMembers,strPrefixArgumentsCopy,strPrefixTable,strPrefixStruct,strPrefixArgumentsReference,strPrefixColumn
        local iIndex,valRes

        -----------------------------------------
        -- 1> Prefix members
        -- Override this prefix if defined
        bFind,strPrefixMembers,strWS3Content = RemoveFirstBloc("PREFIX_MEMBERS",strWS3Content,false)
        if (bFind) then
            strPrefixMembers = Trim(strPrefixMembers) -- Remove all spaces
            print(string.format("Prefix member override: %s",strPrefixMembers))
        else
            strPrefixMembers = "m_" -- m_ by default
        end
        -- 1> Prefix members
        -----------------------------------------

        -----------------------------------------
        -- 2> Prefix members for copy parameters
        -- Override this prefix if needed
        bFind,strPrefixArgumentsCopy,strWS3Content = RemoveFirstBloc("PREFIX_ARGUMENTS_COPY",strWS3Content,false)
        if (bFind) then
            strPrefixArgumentsCopy = Trim(strPrefixArgumentsCopy) -- Remove all spaces
            print(string.format("Prefix argument (copy) override: %s",strPrefixArgumentsCopy))
        else
            strPrefixArgumentsCopy = "_" -- '_' by default
        end
        -- 2> Prefix members for copy parameters
        -----------------------------------------

        -----------------------------------------
        -- 3> Prefix members for reference parameters
        -- Override this prefix if needed
        bFind,strPrefixArgumentsReference,strWS3Content = RemoveFirstBloc("PREFIX_ARGUMENTS_REFERENCE",strWS3Content,false)
        if (bFind) then
            strPrefixArgumentsReference = Trim(strPrefixArgumentsReference) -- Remove all spaces
            print(string.format("Prefix argument (reference) override: %s",strPrefixArgumentsReference))
        else
            strPrefixArgumentsReference = "_r" -- 'r_' by default
        end
        -- 3> Prefix members for reference parameters
        -----------------------------------------

        -----------------------------------------
        -- 4> Prefix table
        -- Override this prefix if needed
        bFind,strPrefixTable,strWS3Content = RemoveFirstBloc("PREFIX_TABLE",strWS3Content,false)
        if (bFind) then
            strPrefixTable = Trim(strPrefixTable) -- Remove all spaces
            print(string.format("Prefix table override: %s",strPrefixTable))
        else
            strPrefixTable = "" -- '' by default
        end
        -- 4> Prefix table
        -----------------------------------------

        -----------------------------------------
        -- 5> Prefix struct
        -- Override this prefix if needed
        bFind,strPrefixStruct,strWS3Content = RemoveFirstBloc("PREFIX_STRUCT",strWS3Content,false)
        if (bFind) then
            strPrefixStruct = Trim(strPrefixStruct) -- Remove all spaces
            print(string.format("Prefix struct override: %s",strPrefixStruct))
        else
            strPrefixStruct = "" -- '' by default
        end
        -- 5> Prefix struct
        -----------------------------------------

        --------------------------------------------------
        -- 6> Prefix column name
        bFind,strPrefixColumn,strWS3Content = RemoveFirstBloc("PREFIX_COLUMN",strWS3Content,false)
        if (bFind) then
            strPrefixColumn = Trim(strPrefixColumn) -- Remove all spaces
            print(string.format("Prefix column override: %s",strPrefixColumn))
        else
            strPrefixColumn = "COLUMN_FIELD_"
        end
        -- 6> Prefix column name
        --------------------------------------------------
        --------------------------------------------------
        -- 7> Table that contains all type with parameters
        -- 7.1> Init default types
        if (strwxOrQToRSTL == "wx") then
            tabTypeMembers.INTEGER              = "etkInt64"
            tabTypeMembers.BIGINT               = "etkInt64"
            tabTypeMembers.VARCHAR              = "etkString"
            tabTypeMembers.DATE                 = "etkDate"
            tabTypeMembers.TIME                 = "etkTime"
            tabTypeMembers.DATETIME             = "etkDateTime"
        else
            tabTypeMembers.INTEGER              = "etkInt64"
            tabTypeMembers.BIGINT               = "etkInt64"
            tabTypeMembers.VARCHAR              = "etkString"
            tabTypeMembers.DATE                 = "etkDate"
            tabTypeMembers.TIME                 = "etkTime"
            tabTypeMembers.DATETIME             = "etkDateTime"
        end

        tabTypeMembers.INT                  = "long"
        tabTypeMembers.UNSIGNED_INT         = "unsigned long"
        tabTypeMembers.SMALLINT             = "short int"
        tabTypeMembers.UNSIGNED_SMALLINT    = "unsigned short int"
        tabTypeMembers.TINYINT              = "char"
        tabTypeMembers.UNSIGNED_TINYINT     = "unsigned char"
        tabTypeMembers.DOUBLE               = "double"
        tabTypeMembers.NUMERIC              = "double"
        tabTypeMembers.BOOLEAN              = "bool"
        tabTypeMembers.BLOB                 = "ETKSQLite3VariantDataBlob"
        tabTypeMembers.VARIANT              = "etkVariant"

        -- 7.2> Override default types by user types
        repeat
            bFind,strContentType,strWS3Content = RemoveFirstBloc("TYPE_MEMBERS",strWS3Content,false)
            if (bFind) then
                local arrResultType = string.split(strContentType,",")
                if (#arrResultType == 2) then
                    local strType = tabTypeMembers[arrResultType[1]]
                    -- Add it: override value or created if not exists
                    tabTypeMembers[Trim(arrResultType[1])] = Trim(arrResultType[2])
                    if (strType ~= nil) then
                        -- Type already exits, override it, log it
                        print(string.format("Type member override: %s=%s",Trim(arrResultType[1]),Trim(arrResultType[2])))
                    else
                        -- Type not exits, add it, log it
                        print(string.format("Type member added: %s=%s",Trim(arrResultType[1]),Trim(arrResultType[2])))
                    end
                else
                    strError = string.format("Type member %s : bad format! Must be $TYPE_MEMBERS${WS3_TYPE,C++ type}",strContentType)
                    bFind = false -- Exit the loop
                end
            end
        until not bFind
        -- 7> Table that contains all type with parameters
        --------------------------------------------------

        --------------------------------------------------
        -- 8> Table that contains all type with parameters
        -- 8.1> Init default types
        -- 8.2> Override default database types by user types
        if (string.len(strError) == 0) then
            repeat
                bFind,strContentType,strWS3Content = RemoveFirstBloc("TYPE_DATABASE",strWS3Content,false)
                if (bFind) then
                    local arrResultType = string.split(strContentType,",")
                    if (#arrResultType == 2) then
                        local strType = tabDatabaseMembers[arrResultType[1]]
                        -- Add it: override value or created if not exists
                        tabDatabaseMembers[Trim(arrResultType[1])] = Trim(arrResultType[2])
                        if (strType ~= nil) then
                            -- Type already exits, override it, log it
                            print(string.format("Database type override: %s=%s",Trim(arrResultType[1]),Trim(arrResultType[2])))
                        else
                            -- Type not exits, add it, log it
                            print(string.format("Database type added: %s=%s",Trim(arrResultType[1]),Trim(arrResultType[2])))
                        end
                    else
                        strError = string.format("Type member %s : bad format! Must be $TYPE_DATABASE${WS3_TYPE,DATABASE TYPE}",strContentType)
                        bFind = false -- Exit the loop
                    end
                end
            until not bFind
        end
        -- 8> Table that contains all type with parameters
        --------------------------------------------------

        --------------------------------------------------
        -- Init reference / value
        --------------------------------------------------
        -- For parameters, as reference
        if (strwxOrQToRSTL == "wx") then
            tabTypeMembersRef.DATE      = "wxdt"
            tabTypeMembersRef.TIME      = "wxdt"
            tabTypeMembersRef.DATETIME  = "wxdt"
            tabTypeMembersRef.BIGINT    = "wxll"
            tabTypeMembersRef.INTEGER   = "wxll"
            tabTypeMembersRef.VARIANT   = "wxVar"
        else
            if (strwxOrQToRSTL == "qt") then
                tabTypeMembersRef.DATE      = "wxdt"
                tabTypeMembersRef.TIME      = "wxdt"
                tabTypeMembersRef.DATETIME  = "wxdt"
                tabTypeMembersRef.BIGINT    = "wxll"
                tabTypeMembersRef.INTEGER   = "wxll"
                tabTypeMembersRef.VARIANT   = "etkVar"
                tabTypeMembersRef.VARIANT   = "etkVar"
            else
                tabTypeMembersRef.DATE      = "d"
                tabTypeMembersRef.TIME      = "t"
                tabTypeMembersRef.DATETIME  = "dt"
                tabTypeMembersRef.BIGINT    = "i64"
                tabTypeMembersRef.INTEGER   = "i64"
                tabTypeMembersRef.VARIANT   = "etkVar"
            end
        end
        -- For parameters, as reference
        tabTypeMembersRef.BLOB              = "blb"

        -- For parameters, as value
        tabTypeMembersCopy.INT                  = "l"
        tabTypeMembersCopy.UNISGNED_INT         = "ul"
        tabTypeMembersCopy.SMALLINT             = "si"
        tabTypeMembersCopy.UNSIGNED_SMALLINT    = "usi"
        tabTypeMembersCopy.TINYINT              = "by"
        tabTypeMembersCopy.UNSIGNED_TINYINT     = "uby"
        tabTypeMembersCopy.VARCHAR              = "str"
        tabTypeMembersCopy.DOUBLE               = "d"
        tabTypeMembersCopy.NUMERIC              = "d"
        tabTypeMembersCopy.BOOLEAN              = "b"
        tabTypeMembersCopy.VARIANT              = "v"
        -------------------------------------------------------
        -- 9> Reference - Override default types by user types
        --                or add new one
        if (string.len(strError) == 0) then
            repeat
                bFind,strContentType,strWS3Content = RemoveFirstBloc("PREFIX_REF",strWS3Content,false)
                if (bFind) then
                    local arrResultType = string.split(strContentType,",")
                    if (#arrResultType == 2) then
                        -- Add it: override value or created if not exists
                        print(string.format("=================================>: %s = %s",Trim(arrResultType[1]),Trim(arrResultType[2])))
                        iIndex2Remove = indexOf(tabTypeMembersCopy, Trim(arrResultType[1]))
                        tabTypeMembersRef[Trim(arrResultType[1])] = Trim(arrResultType[2])
                        if (iIndex2Remove ~= nil) then
                            table.remove(tabTypeMembersCopy, iIndex2Remove) -- Remove it from value
                        end
                        print(string.format("Prefix refefence: %s = %s",Trim(arrResultType[1]),Trim(arrResultType[2])))
                    else
                        strError = string.format("Prefix reference %s : bad format! Must be $PREFIX_REF${WS3_TYPE,C++ type}",strContentType)
                        bFind = false -- Exit the loop
                    end
                end
            until not bFind
        end
        -- 9> Reference - Override default types by user types
        -------------------------------------------------------

        -------------------------------------------------------
        -- 10> Copy - Override default types by user types
        --           or add new one
        if (string.len(strError) == 0) then
            repeat
                bFind,strContentType,strWS3Content = RemoveFirstBloc("PREFIX_COPY",strWS3Content,false)
                if (bFind) then
                    local arrResultType = string.split(strContentType,",")
                    if (#arrResultType == 2) then
                        -- Add it: override value or created if not exists
                        tabTypeMembersCopy[Trim(arrResultType[1])] = Trim(arrResultType[2])
                        tabTypeMembersRef[Trim(arrResultType[1])] = nil -- Remove it from value
                        print(string.format("Prefix copy: %s = %s",Trim(arrResultType[1]),Trim(arrResultType[2])))
                    end
                end
            until not bFind
        end

        -- 10> Copy - Override default types by user types
        --           or add new one
        -------------------------------------------------------

        --------------------------------------------------
        -- 11> Table that contains all binding
        if (string.len(strError) == 0) then
            repeat
                bFind,strContentType,strWS3Content = RemoveFirstBloc("TYPE_BIND",strWS3Content,false)
                if (bFind) then
                    local arrResultType = string.split(strContentType,",")
                    if (#arrResultType == 2) then
                        print(string.format("Bind: %s=%s", Trim(arrResultType[1]),Trim(arrResultType[2])))
                        -- Add it: override value or created if not exists
                        tabTypeBind[Trim(arrResultType[1])] =  Trim(arrResultType[2])
                    else
                        strError = string.format("Type member %s : bad format! Must be $TYPE_BIND${WS3_TYPE,BIND TYPE}",strContentType)
                        bFind = false -- Exit the loop
                    end
                end
            until not bFind
        end
        -- 11> Table that contains all binding
        --------------------------------------------------

        -----------------------------------------
        -- 12> Export / Import macro
        bFind,strExportImport,strWS3Content = RemoveFirstBloc("EXPORT_IMPORT",strWS3Content,false)
        if (bFind) then
            strExportImport = Trim(strExportImport) .. " "  -- Remove all spaces but let one at right
            print(string.format("Export / Import override: %s",strExportImport))
        else
            strExportImport = "" -- '_' by default
        end
        -- 12> xport / Import macro
        -----------------------------------------

        ----------------------------------------------------------------
        -- 13> Table that contains all type used as function parameters
        -- 13.1> Init default types
        if (strwxOrQToRSTL == "wx") then
            tabTypeArgs.INTEGER  = "wxLongLong"
            tabTypeArgs.BIGINT   = "wxLongLong"
            tabTypeArgs.INT      = "long"
            tabTypeArgs.SMALLINT = "short int"
            tabTypeArgs.TINYINT  = "char"
            tabTypeArgs.VARCHAR  = "wxString"
            tabTypeArgs.DOUBLE   = "double"
            tabTypeArgs.NUMERIC  = "double"
            tabTypeArgs.BOOLEAN  = "bool"
            tabTypeArgs.DATE     = "wxDateTime"
            tabTypeArgs.TIME     = "wxDateTime"
            tabTypeArgs.DATETIME = "wxDateTime"
            tabTypeArgs.BLOB     = "ETKSQLite3VariantDataBlob"
            tabTypeArgs.VARIANT  = "etkVariant"
        else
            if (strwxOrQToRSTL == "qt") then -- QT
                tabTypeArgs.INTEGER  = "qint64"
                tabTypeArgs.BIGINT   = "qint64"
                tabTypeArgs.INT      = "long"
                tabTypeArgs.SMALLINT = "short int"
                tabTypeArgs.TINYINT  = "char"
                tabTypeArgs.VARCHAR  = "QString"
                tabTypeArgs.DOUBLE   = "double"
                tabTypeArgs.NUMERIC  = "double"
                tabTypeArgs.BOOLEAN  = "bool"
                tabTypeArgs.DATE     = "QDate"
                tabTypeArgs.TIME     = "QTime"
                tabTypeArgs.DATETIME = "QDateTime"
                tabTypeArgs.BLOB     = "ETKSQLite3VariantDataBlob"
                tabTypeArgs.VARIANT  = "etkVariant"
            else
                tabTypeArgs.INTEGER  = "etkInt64"
                tabTypeArgs.BIGINT   = "etkInt64"
                tabTypeArgs.INT      = "long"
                tabTypeArgs.SMALLINT = "short int"
                tabTypeArgs.TINYINT  = "char"
                tabTypeArgs.VARCHAR  = "etkString"
                tabTypeArgs.DOUBLE   = "double"
                tabTypeArgs.NUMERIC  = "double"
                tabTypeArgs.BOOLEAN  = "bool"
                tabTypeArgs.DATE     = "etkDate"
                tabTypeArgs.TIME     = "etkTime"
                tabTypeArgs.DATETIME = "etkDateTime"
                tabTypeArgs.BLOB     = "ETKSQLite3VariantDataBlob"
                tabTypeArgs.VARIANT  = "etkVariant"
            end
        end

        -- 13.2> Override default types by user types
        repeat
            bFind,strContentType,strWS3Content = RemoveFirstBloc("TYPE_ARGS",strWS3Content,false)
            if (bFind) then
                local arrResultType = string.split(strContentType,",")
                if (#arrResultType == 2) then
                    local strType = tabTypeArgs[arrResultType[1]]
                    -- Add it: override value or created if not exists
                    tabTypeArgs[Trim(arrResultType[1])] = Trim(arrResultType[2])
                    if (strType ~= nil) then
                        -- Type already exits, override it, log it
                        print(string.format("Type member override: %s=%s",Trim(arrResultType[1]),Trim(arrResultType[2])))
                    else
                        -- Type not exits, add it, log it
                        print(string.format("Type member added: %s=%s",Trim(arrResultType[1]),Trim(arrResultType[2])))
                    end
                else
                    strError = string.format("Type member %s : bad format! Must be $TYPE_ARGS${WS3_TYPE,C++ type}",strContentType)
                    bFind = false -- Exit the loop
                end
            end
        until not bFind
        -- 13> Table that contains all type used as function parameters
        ----------------------------------------------------------------

        ----------------------------------------------------------------
        -- 14> Table that contains all prefix to use into stucture
        -- 14.1> Init default types
        if (strwxOrQToRSTL == "qt") then
            tabPrefixTypeMembers.wxLongLong = "wxll"
            tabPrefixTypeMembers.wxDateTime = "wxdt"
        else
            if (strwxOrQToRSTL == "stl") then
                tabPrefixTypeMembers.wxLongLong = "etkll"
                tabPrefixTypeMembers.wxDateTime = "etkdt"
            end
        end

        -- 14.2> Override default types by user types
        repeat
            bFind,strContentType,strWS3Content = RemoveFirstBloc("PREFIX_TYPE_MEMBERS",strWS3Content,false)
            if (bFind) then
                local arrResultType = string.split(strContentType,",")
                if (#arrResultType == 2) then
                    local strType = tabPrefixTypeMembers[arrResultType[1]]
                    -- Add it: override value or created if not exists
                    tabPrefixTypeMembers[Trim(arrResultType[1])] = Trim(arrResultType[2])
                    if (strType ~= nil) then
                        -- Type already exits, override it, log it
                        print(string.format("Prefix type member override: %s=%s",Trim(arrResultType[1]),Trim(arrResultType[2])))
                    else
                        -- Type not exits, add it, log it
                        print(string.format("Prefix type member added: %s=%s",Trim(arrResultType[1]),Trim(arrResultType[2])))
                    end
                else
                    strError = string.format("Prefix type member %s : bad format! Must be $PREFIX_TYPE_MEMBERS${C++ Type,prefix}",strContentType)
                    bFind = false -- Exit the loop
                end
            end
        until not bFind

        -- 14> Find Table comments
        ----------------------------------------------------------------
        --
        -- Close brace (end table description) (not captured) <-------------------------------------------------------
        -- All columns descriptions <-----------------------------------------------------------------------------   |
        -- Open brace (begin table description) (not captured) <------------------------------------             |   |
        -- Table name <-----------------------------------------------------------                 |             |   |
        -- Beginning of table comment<--------------    open brace <-------      |      -> close brace           |   |
        --                               __________|______________        |  ____|____  |          |           __|__ |
        --                             /                          \       | /         \ |          |          /     \|(end)
        local regExpression =         "ETK_SQLITE3_COMMENT_TABLE[ \t]*%(([a-zA-Z_]+)%)[ \t\r\n]*{[ \t\r\n]*([^}]+)}"
        local iCommentBegin, iCommentEnd
        local strTableName,strColumnComment
        local dicCommentTables = {}
        iCommentBegin, iCommentEnd, strTableName, strColumnComment = string.find(strWS3Content, regExpression, iCommentBegin)
        while iCommentBegin ~= nil do
            local indexBegin, indexEnd = ComputeIndexes2RemoveBlankLine(strColumnComment, 0, string.len(strColumnComment) - 1, true, false)
            dicCommentTables[strTableName] = string.sub(strColumnComment, indexBegin, indexEnd)
            iCommentBegin, iCommentEnd = ComputeIndexes2RemoveBlankLine(strWS3Content, iCommentBegin - 1, iCommentEnd + 1, false, true)
            strWS3Content = string.sub(strWS3Content, 0, iCommentBegin) .. string.sub(strWS3Content, iCommentEnd)
            iCommentBegin, iCommentEnd, strTableName, strColumnComment = string.find(strWS3Content, regExpression, iCommentBegin)
        end

        -- 15> Table that contains all type used as function parameters
        ----------------------------------------------------------------

        --
        -- Close brace (end table description) (not captured) <---------------------------------------------------------------
        -- All columns descriptions <-------------------------------------------------------------------------------------   |
        -- Open brace (begin table description) (not captured) <--------------------------------------------             |   |
        -- Table name <-------------------------------------------------------------------                 |             |   |
        -- Beginning of table <--------------------------------    open brace <----      |      -> close brace           |   |
        --                                          __________|___________        |  ____|___   |          |            _|_  |
        --                                        /                       \       | /        \  |          |           /   \ |(end)
              regExpression            =         "ETK_SQLITE3_DECLARE_TABLE[ \t]*%(([a-zA-Z_]+)%)[ \t\r\n]*{[ \t\r\n]*([^}]+)}"
        local strRegexSearchForeignKey = "DECLARE_FOREIGN_KEY%(([%a_][%a%d_]*)%.([%a_][%a%d]*)%)" -- to find foreign key
        local strRegexSearchIndex      = "DECLARE_INDEX%(([^)]+)%)" -- to find index
        local strRegexSearchDataImplementation = "DECLARE_DATA_IMPLEMENTATION%([ \t]*([%a%d_ %.\\/]*)[\t]*,[ \t]*([%a%d_ %.\\/]*[\t]*)%)" -- to find implentation files for structure
        local iExpressionBegin, iLastExpressionBegin, iExpressionEnd = 1,1
        local indexBegin,indexEnd,strColumnDescriptions
        local strCPPColumnImplementation,strCPPColumnAdd,strCPPColumnDeclaration,strCPPColumnVariablesDeclaration,strCPPConstructorVariablesImplementation
        local strHDeclareVariablesGetterSetters,strCPPImplementVariablesGetterSetters,strCPPTableConstruction,strCPPTableConstructionFK,strCPPTableConstructionPK,strCPPTableConstructionINDEX
        local iNbPrimaryKey,bHasAutoincrementPrimaryKey
        local dicColumnsName -- Columns names to check INDEX columns
        local strTemplateHeaderFileCopy,strTemplateSourceFileCopy
        local iNbTableFound = 0

        -- Extract table to generate
        iExpressionBegin, iExpressionEnd, strTableName, strColumnDescriptions = string.find(strWS3Content, regExpression, iExpressionEnd)
        -- Read all tables
        while string.len(strError) == 0 and iExpressionBegin ~= nil do
            -- New Table, reset implementations
            strCPPColumnImplementation                  = ""
            strCPPColumnAdd                             = ""
            strCPPColumnDeclaration                     = ""
            strCPPColumnVariablesDeclaration            = ""
            strCPPConstructorVariablesImplementation    = ""
            strHDeclareVariablesGetterSetters           = ""
            strCPPImplementVariablesGetterSetters       = ""
            strCPPTableConstruction                     = "" -- To create the SQL used to create the table
            strCPPTableConstructionFK                   = "" -- To create the SQL used to create the table (Foreign key)
            strCPPTableConstructionPK                   = "" -- To create the SQL used to create the primary key
            strCPPTableConstructionINDEX                = "" -- To create the SQL used to create the index
            iNbPrimaryKey                               = 0
            bHasAutoincrementPrimaryKey                 = false
            strTemplateHeaderFileCopy                   = strTemplateHeaderFile
            strTemplateSourceFileCopy                   = strTemplateSourceFile
            bUserStructImplementation                   = false
            dicColumnsName                              = {}

            -- Table name
            strCPPColumnDeclaration = ""
            iNbTableFound = iNbTableFound + 1

            iBracket = 0
            local strResults = ""
            local strImplementInitVariableConstructor   = "";
            -------------------------------------------
            -- Replace all spaces into [] bloc with '@'
            for iIndexStr = 1,string.len(strColumnDescriptions) do
                if (iBracket <= 0 or string.sub(strColumnDescriptions,iIndexStr,iIndexStr)) ~= " " then
                    strResults = strResults .. string.sub(strColumnDescriptions,iIndexStr,iIndexStr)
                    if (string.sub(strColumnDescriptions,iIndexStr,iIndexStr) == "[") then
                        iBracket = iBracket + 1
                    elseif (string.sub(strColumnDescriptions,iIndexStr,iIndexStr) == "]") then
                        iBracket = iBracket - 1
                    end
                else
                    strResults = strResults .. "@"
                end
            end
            -- Replace all spaces into [] bloc with '@'
            -------------------------------------------

            -- Prepare the table bloc format
            strResults = "\n" .. strResults .. "\n"                     -- <-- To be sure the split function give all tokens
            strResults = string.gsub(strResults,"\r","\n")              -- <-- Remove all \r char, replace by \n
            strResults = string.replaceall(strResults,"\n\n","\n")      -- <-- Keep only one \n, replace all twice char (or more) by one only
            strResults = string.replaceall(strResults,"\n"," $$**$$ ")  -- <-- Make beginning tag in place of \n
            strResults = string.gsub(strResults,"\t"," ")               -- <-- Remove TAB char
            strResults = string.replaceall(strResults,"  "," ")         -- <-- Keep only one space to split the fields
            strResults = Trim(strResults)                               -- <-- Remove spaces in begin or end of the string
            local bIsCol, bIsForeignKey, bIsIndex, strColumName, strType, strCppType, bIsNull, bIsNotNull, bIsPrimary, bIsUnique, bIsAutoIncrement
            local lstIndex = {}
            local iIndexInColums, iLine , bOnDeleteCascade, bOnUpdateCascade, bOnDeleteSetNull, bOnUpdateSetNull, strFKTable, strFKColumn, bUserStructImplementation
            iLine     = 0

            local tResults = string.split(strResults," ")
            for iIndex,strResult in ipairs(tResults) do
                -- Read all lines : decode table
                if (strResult == "$$**$$" or iLine==0) then
                    -- Start a new column declaration
                    if ((iLine ~= 0 and not bUserStructImplementation) or (iLine ~= 1 and bUserStructImplementation)) then
                        if (not bIsIndex) then
                            if ((iLine ~= 1 and not bUserStructImplementation) or (iLine > 2 and bUserStructImplementation)) then
                                -- If not first line
                                strCPPColumnImplementation               = strCPPColumnImplementation .. "\n"
                                strCPPColumnAdd                          = strCPPColumnAdd .. "\n"
                                strCPPColumnDeclaration                  = strCPPColumnDeclaration .. "\n"
                                strCPPColumnVariablesDeclaration         = strCPPColumnVariablesDeclaration .. "\n"
                                strHDeclareVariablesGetterSetters        = strHDeclareVariablesGetterSetters .. "\n\n"
                                strCPPImplementVariablesGetterSetters    = strCPPImplementVariablesGetterSetters .. "\n"
                                strCPPTableConstruction                  = strCPPTableConstruction .. "\n"
                            end

                            local strPrefixType,strPrefixTypeMember = "",""
                            if (tabTypeMembersRef[strType] ~= nil) then
                                -- Member type to pass as reference
                                strPrefixType = tabTypeMembersRef[strType]
                            elseif (tabTypeMembersCopy[strType] ~= nil) then
                                -- Member type to pass as copy
                                strPrefixType = tabTypeMembersCopy[strType]
                            end

                            strPrefixTypeMember = tabPrefixTypeMembers[strCppType]
                            if (strPrefixTypeMember == nil) then
                                strPrefixTypeMember = strPrefixType -- If not defined, same as prefix type
                            end

                            --------------------------
                            -- Source code generation
                            -- Contruct the SQL string used to build the table
                            local strDatabaseType = tabDatabaseMembers[strType] or strType -- tabDatabaseMembers[strType] if defined, strType else

                            strCPPTableConstruction     =   strCPPTableConstruction ..
                                                            string.format("%s\"%s %s%s%s%s%s%s,\""
                                                                          ,string.rep(" ",22 + string.len(strTableName))
                                                                          ,strColumName
                                                                          ,strDatabaseType
                                                                          ,(bIsPrimary and bIsAutoIncrement) and " PRIMARY KEY" or "" -- bIsPrimary ? "PRIMARY" : ""
                                                                          ,bIsNull and " NULL" or ""                        -- bIsNull ? "NULL" : ""
                                                                          ,bIsNotNull and " NOT NULL" or ""                 -- bIsNotNull ? "NOT NULL" : ""
                                                                          ,bIsAutoIncrement and " AUTOINCREMENT" or ""      -- bIsAutoIncrement ? "AUTOINCREMENT" : ""
                                                                          ,bIsUnique and " UNIQUE" or ""                    -- bIsUnique ? "UNIQUE" : ""
                                                                         )

                            -- Compute primary key needed
                            if (bIsPrimary) then
                                iNbPrimaryKey = iNbPrimaryKey + 1
                                if (not bIsAutoIncrement) then
                                    -- Add all primary keys only if no AUTOINCREMENT field
                                    strCPPTableConstructionPK = strCPPTableConstructionPK ..
                                                                string.format("%s%s"
                                                                              ,(string.len(strCPPTableConstructionPK) ~= 0) and "," or ""
                                                                              ,strColumName
                                                                             )
                                else
                                    bHasAutoincrementPrimaryKey = true
                                end
                            end

                            -- Compute Foreign key / INDEX if needed
                            if (bIsCol == false) then
                                if (bIsForeignKey == true) then
                                    -- Only for foreign key
                                    strCPPTableConstructionFK = strCPPTableConstructionFK ..
                                                                string.format("\n%s\"  FOREIGN KEY(%s)\"\n%s\"    REFERENCES %s(%s)%s%s%s%s,\""
                                                                              ,string.rep(" ",22 + string.len(strTableName))
                                                                              ,strColumName
                                                                              ,string.rep(" ",22 + string.len(strTableName))
                                                                              ,strFKTable
                                                                              ,strFKColumn
                                                                              ,bOnDeleteCascade and string.format("\"\n%s\"      ON DELETE CASCADE",string.rep(" ",22 + string.len(strTableName))) or ""
                                                                              ,bOnDeleteSetNull and string.format("\"\n%s\"      ON DELETE SET NULL",string.rep(" ",22 + string.len(strTableName))) or ""
                                                                              ,bOnUpdateCascade and string.format("\"\n%s\"      ON UPDATE CASCADE",string.rep(" ",22 + string.len(strTableName))) or ""
                                                                              ,bOnUpdateSetNull and string.format("\"\n%s\"      ON UPDATE SET NULL",string.rep(" ",22 + string.len(strTableName))) or ""
                                                                             )
                                end
                            end

                            strCPPColumnImplementation  =   strCPPColumnImplementation ..
                                                            string.format("const ETKSQLite3Column %s%s::%s%s%s= ETKSQLite3Column(TABLE_NAME,_T(\"%s\"));"
                                                                          ,strPrefixTable
                                                                          ,strTableName
                                                                          ,strPrefixColumn
                                                                          ,strColumName
                                                                          ,string.rep(" ",math.max(1,16 - string.len(strColumName) - (string.len(strTableName) % 4)))
                                                                          ,strColumName
                                                                         )

                            if (tabTypeBind[strType] ~= nil) then
                                -- Binding type
                                strCPPColumnAdd = strCPPColumnAdd ..
                                                  string.format("    AddColumn(ETKSQLite3Column(%s%s,%sETKSQLite3ColumnAttributes(%s(&%s%s%s)%s)));"
                                                                ,strPrefixColumn
                                                                ,strColumName
                                                                ,string.rep(" ",math.max(1,17 - string.len(strColumName)))
                                                                ,tabTypeBind[strType]
                                                                ,strPrefixMembers             -- m_ or other if override
                                                                ,strPrefixTypeMember          -- l for integer (long), str for string, ... other if override
                                                                ,strColumName
                                                                ,(bIsPrimary and (bIsAutoIncrement or strDatabaseType=="INTEGER")) and ",false" or ""  -- Add this column when adding new record ? No for primary integer primary key
                                                               )
                            else
                                -- No binding type
                                strCPPColumnAdd = strCPPColumnAdd ..
                                                  string.format("    AddColumn(ETKSQLite3Column(%s%s,%sETKSQLite3ColumnAttributes(&%s%s%s%s)));"
                                                                ,strPrefixColumn
                                                                ,strColumName
                                                                ,string.rep(" ",math.max(1,17 - string.len(strColumName)))
                                                                ,strPrefixMembers             -- m_ or other is override
                                                                ,strPrefixTypeMember          -- l for integer (long), str for string, ... other if override
                                                                ,strColumName
                                                                ,(bIsPrimary and (bIsAutoIncrement or strDatabaseType=="INTEGER")) and ",false" or ""  -- Add this column when adding new record ? No for primary integer primary key
                                                             )
                            end

                            -- The structure that contains data could be a wxString and all getter and setter could be
                            -- QString (for example)
                            local strTypeArg = tabTypeArgs[strType]

                            if (strTypeArg == nil) then
                                strTypeArg = tabTypeMembers[strType]
                            end

                            if (tabTypeMembersRef[strType] ~= nil) then
                                -- Getter and setter with Reference
                                local strParamArgument = strPrefixArgumentsReference .. tabTypeMembersRef[strType] .. strColumName

                                strHDeclareVariablesGetterSetters = strHDeclareVariablesGetterSetters ..
                                                                    string.format("    const %s &%sGet%s() const;\n    void %sSet%s(const %s &%s);"
                                                                                  ,strTypeArg
                                                                                  ,string.rep(" ",math.max(1,28 - string.len(strTypeArg)))
                                                                                  ,strColumName
                                                                                  ,string.rep(" ",31)
                                                                                  ,strColumName
                                                                                  ,strTypeArg
                                                                                  ,strParamArgument
                                                                                 )

                                strCPPImplementVariablesGetterSetters = strCPPImplementVariablesGetterSetters ..
                                                                        string.format("const %s &%s%s::Get%s() const\n{\n    return %s%s%s;\n}\n\nvoid %s%s::Set%s(const %s &%s)\n{\n    %s%s%s = %s;\n}\n"
                                                                                      ,strTypeArg
                                                                                      ,strPrefixTable
                                                                                      ,strTableName
                                                                                      ,strColumName
                                                                                      ,strPrefixMembers
                                                                                      ,strPrefixTypeMember
                                                                                      ,strColumName
                                                                                      ,strPrefixTable
                                                                                      ,strTableName
                                                                                      ,strColumName
                                                                                      ,strTypeArg
                                                                                      ,strParamArgument
                                                                                      ,strPrefixMembers
                                                                                      ,strPrefixTypeMember
                                                                                      ,strColumName
                                                                                      ,strParamArgument
                                                                                     )
                            else -- if (tabTypeMembersRef[strType] ~= nil) then
                                -- // Getter and setter by copy
                                local strParamArgument = ""
                                if (tabTypeMembersCopy[strType] ~= nil) then
                                    strParamArgument = tabTypeMembersCopy[strType]
                                end
                                strParamArgument = strPrefixArgumentsCopy .. strParamArgument .. strColumName

                                strHDeclareVariablesGetterSetters = strHDeclareVariablesGetterSetters ..
                                                                    string.format("    %s%sGet%s() const;\n    void %sSet%s(%s %s);"
                                                                                  ,strTypeArg
                                                                                  ,string.rep(" ",math.max(1,36 - string.len(strTypeArg)))
                                                                                  ,strColumName
                                                                                  ,string.rep(" ",31)
                                                                                  ,strColumName
                                                                                  ,strTypeArg
                                                                                  ,strParamArgument
                                                                                 )

                                strCPPImplementVariablesGetterSetters = strCPPImplementVariablesGetterSetters ..
                                                                        string.format("%s %s%s::Get%s() const\n{\n    return %s%s%s;\n}\n\nvoid %s%s::Set%s(%s %s)\n{\n    %s%s%s = %s;\n}\n"
                                                                                      ,strTypeArg
                                                                                      ,strPrefixTable
                                                                                      ,strTableName
                                                                                      ,strColumName
                                                                                      ,strPrefixMembers
                                                                                      ,strPrefixTypeMember
                                                                                      ,strColumName
                                                                                      ,strPrefixTable
                                                                                      ,strTableName
                                                                                      ,strColumName
                                                                                      ,strTypeArg
                                                                                      ,strParamArgument
                                                                                      ,strPrefixMembers
                                                                                      ,strPrefixTypeMember
                                                                                      ,strColumName
                                                                                      ,strParamArgument
                                                                                     )
                            end -- if (tabTypeMembersRef[strType] ~= nil) then / else

                            -- If not user implementation, remove only tags, else it is already done when user implementation
                            -- has been read
                            if (bUserStructImplementation) then
                                local setReplace = {["COLUMN_NAME"]   = strColumName,
                                                    ["VARIABLE_NAME"] = strPrefixMembers .. strPrefixTypeMember .. strColumName,
                                                    ["VARIABLE_TYPE"] = strTypeArg}
                                strTemplateHeaderFileCopy = ReplaceRepeatSection(setReplace,strTemplateHeaderFileCopy);
                                strTemplateSourceFileCopy = ReplaceRepeatSection(setReplace,strTemplateSourceFileCopy);
                            end

                            strCPPColumnDeclaration = strCPPColumnDeclaration .. string.format("    static const ETKSQLite3Column       %s%s;",strPrefixColumn,strColumName)
                            -- Add Datas used to make the binding
                            strCPPColumnVariablesDeclaration = strCPPColumnVariablesDeclaration ..
                                                               string.format("    %s%s%s%s%s;"
                                                                             ,strCppType
                                                                             ,string.rep(" ",math.max(1,36 - string.len(strCppType)))
                                                                             ,strPrefixMembers
                                                                             ,strPrefixTypeMember
                                                                             ,strColumName
                                                                            )

                            if (string.len(strImplementInitVariableConstructor) ~= 0) then
                                -- If initialisation is set, add it
                                strCPPConstructorVariablesImplementation = strCPPConstructorVariablesImplementation ..
                                                                           string.format("    %s%s%s%s;\n"
                                                                                         ,strPrefixMembers
                                                                                         ,strPrefixTypeMember
                                                                                         ,strColumName
                                                                                         ,strImplementInitVariableConstructor
                                                                                        )
                            end
                        end -- else
                    end -- if ((iLine ~= 0 and not bUserStructImplementation) or (iLine ~= 1 and bUserStructImplementation)) then / else
                    -- Set Column default values
                    bIsCol                    = nil -- null: no type, true: column else other
                    bIsForeignKey             = nil -- true: foreign key
                    bIsIndex                  = false -- true: index
                    lstIndex                  = {}
                    strColumName              = nil
                    strType                   = nil
                    strCppType                = nil
                    bIsNull                   = false
                    bIsNotNull                = false
                    bIsPrimary                = false
                    bIsUnique                 = false
                    bIsAutoIncrement          = false
                    bOnDeleteCascade          = false
                    bOnDeleteSetNull          = false
                    bOnUpdateCascade          = false
                    bOnUpdateSetNull          = false
                    strDataHeaderImpl         = nil
                    strDataSourceImpl         = nil
                    iIndexInColums   = 0
                    strImplementInitVariableConstructor = ""
                    iLine = iLine + 1
                else -- if strResult == "$$**$$" or iLine==0
                    switch(iIndexInColums) : caseof
                    {
                        [0]     =   function(x) -- DECLARE_COLUMN or DECLARE_FOREIGN_KEY or DECLARE_INDEX
                                        if (strResult == "DECLARE_COLUMN") then
                                            bIsCol = true
                                        elseif (string.find(strResult,strRegexSearchForeignKey)) then
                                            -- Find table & column name
                                            local iFKStart,iFKEnd
                                            iFKStart,iFKEnd,strFKTable,strFKColumn = string.find(strResult,strRegexSearchForeignKey)
                                            bIsCol = false -- It is a foreign key
                                            bIsForeignKey = true
                                        elseif (string.find(strResult,strRegexSearchIndex)) then
                                            -- Find column(s) name(s)
                                            local iFKStart,iFKEnd, strAllColumns
                                            iFKStart,iFKEnd,strAllColumns = string.find(strResult,strRegexSearchIndex)
                                            for iIndex,strResult in ipairs(string.split(strAllColumns,",")) do
                                                strResult = Trim(strResult)
                                                if (not dicColumnsName[strResult]) then
                                                    -- Error
                                                    strError = string.format("Table %s - line %d (DECLARE_INDEX(%s) : the columns '%s' is unknown!",strTableName, iLine, strAllColumns, strResult)
                                                    break
                                                end
                                            end
                                            table.insert(lstIndex, strAllColumns) -- The last index contains all coluums
                                            bIsCol = false -- It is a Index
                                            bIsIndex = true
                                        elseif (string.find(strResult,strRegexSearchDataImplementation)) then
                                            -- Find data implementation
                                            local iStartDataImpl,iEndDataImpl,bReadOK,strDataHeaderImplFile,strDataSourceImplFile
                                            local strDataHeaderImpl,strDataSourceImpl
                                            iStartDataImpl,iEndDataImpl,strDataHeaderImplFile,strDataSourceImplFile = string.find(strResult,strRegexSearchDataImplementation)
                                            -- If implementation
                                            strDataHeaderImplFile = Trim(strDataHeaderImplFile)
                                            strDataSourceImplFile = Trim(strDataSourceImplFile)
                                            bReadOK, strDataHeaderImpl =  ReadFileContent(strDataHeaderImplFile)
                                            if (not bReadOK) then
                                                -- Error
                                                strError = string.format("Table %s - line %d (DECLARE_DATA_IMPLEMENTATION='%s') : cannot read file '%s'!",strTableName,iLine,strResult,strDataHeaderImplFile)
                                            else
                                                bReadOK, strDataSourceImpl =  ReadFileContent(strDataSourceImplFile)
                                                if (bReadOK) then
                                                    strTemplateHeaderFileCopy = string.gsub(strTemplateHeaderFileCopy,"$STRUCT_IMPLEMENTATION%$",strDataHeaderImpl .. "$STRUCT_IMPLEMENTATION$")
                                                    strTemplateHeaderFileCopy = RemoveAllBlocs("STRUCT_IMPLEMENTATION",strTemplateHeaderFileCopy,false)
                                                    strTemplateSourceFileCopy = string.gsub(strTemplateSourceFileCopy,"$STRUCT_IMPLEMENTATION%$",strDataSourceImpl .. "$STRUCT_IMPLEMENTATION$")
                                                    strTemplateSourceFileCopy = RemoveAllBlocs("STRUCT_IMPLEMENTATION",strTemplateSourceFileCopy,false)
                                                    bUserStructImplementation = true
                                                    print(string.format("User implementation for class %s : %s / %s",strTableName,strDataHeaderImplFile,strDataSourceImplFile))
                                                else
                                                    -- Error
                                                    strError = string.format("Table %s - line %d (DECLARE_DATA_IMPLEMENTATION='%s') : cannot read file '%s'!",strTableName,iLine,strResult,strDataSourceImplFile)
                                                end
                                            end
                                            bIsCol = false -- It is an implementation sources
                                        else
                                            -- Error
                                            strError = string.format("Table %s - line %d: '%s' is unknown, waiting DECLARE_COLUMN or DECLARE_FOREIGN_KEY(table.col) or DECLARE_INDEX(col1[,col2,col3...]) or DECLARE_DATA_IMPLEMENTATION(template header file, template source file)",strTableName,iLine,strResult)
                                        end
                                    end,
                        [1]     =   function(x) -- Column name
                                        strColumName = strResult
                                        -- Add to list columns name list to check INDEX
                                        if (bIsIndex == false) then
                                            dicColumnsName[strColumName] = true
                                        else
                                            strCPPTableConstructionINDEX = strCPPTableConstructionINDEX ..
                                                                           string.format("\n        \"CREATE INDEX %s ON %s(%s);\""
                                                                                         ,strColumName
                                                                                         ,strTableName
                                                                                         ,lstIndex[#lstIndex]
                                                                                        )
                                            print(string.format("Index %s ON %s(%s)",strColumName,strTableName,lstIndex[#lstIndex]))
                                        end
                                        if (strDataSourceImpl ~= nil) then
                                            -- Error
                                            strError = string.format("Table %s - line %d: after DECLARE_DATA_IMPLEMENTATION, no more information is needed!",strTableName,iLine)
                                        end
                                    end,
                        [2]     =   function(x) -- Type
                                        if (bIsIndex == true) then
                                            -- Error
                                            strError = string.format("Table %s - line %d (column=DECLARE_INDEX(%s) %s : too much columns for INDEX declaration!", strTableName, iLine, lstIndex[#lstIndex], strColumName)
                                        else
                                            if (tabTypeMembers[strResult] ~= nil) then
                                                -- Test if this type is supported
                                                strType = strResult
                                                strCppType = tabTypeMembers[strType] -- C++ type for this column type
                                            else
                                                -- Error
                                                strError = string.format("Table %s - line %d (column='%s') : type '%s' is unknown!",strTableName,iLine,strColumName,strResult)
                                            end
                                        end
                                    end,
                        default =   function(x) -- Other keywords
                                        if (strResult == "PRIMARY_KEY") then
                                            if (iIndexInColums == 3) then
                                                bIsPrimary = true
                                            else
                                                -- Error
                                                strError = string.format("Table %s - line %d (column='%s') '%s': must be defined after colum type",strTableName,iLine,strColumName,strResult)
                                            end
                                        elseif (strResult == "NOT_NULL" or strResult == "NULL")  then
                                            if (bIsNull or bIsNotNull)  then
                                                -- Error
                                                strError = string.format("Table %s - line %d (column='%s'): attribute '%s' NOT_NULL or NULL defined more than once!",strTableName,iLine,strColumName,strResult)
                                            elseif (bIsPrimary) then
                                                -- Error
                                                strError = string.format("Table %s - line %d (column='%s'): attribute '%s' not allowed for PRIMARY_KEY (always NOT_NULL)",strTableName,iLine,strColumName,strResult)
                                            else
                                                if (strResult == "NOT_NULL") then
                                                    bIsNotNull = true
                                                else -- strResult == "NULL"
                                                    bIsNull = true
                                                end
                                            end
                                        elseif (strResult == "AUTOINCREMENT") then
                                            bIsAutoIncrement = true
                                        elseif (strResult == "ON_DELETE_CASCADE") then
                                            if (bIsCol == false and bIsForeignKey == true) then
                                                -- it is a foreign key, delete is accepted
                                                if (bOnDeleteSetNull) then
                                                    -- Error, using both not compatible delete
                                                    strError = string.format("Table %s - line %d (column='%s'): attribute '%s' using both ON_DELETE_CASCADE and ON_DELETE_SET_NULL is not allowed!",strTableName,iLine,strColumName,strResult)
                                                else
                                                    bOnDeleteCascade = true
                                                end
                                            else
                                                -- Error
                                                strError = string.format("Table %s - line %d (column='%s'): attribute '%s' not allowed for a DECLARE_COLUMN column, only avalaible for foreign key, use DECLARE_FOREIGN_KEY instead!",strTableName,iLine,strColumName,strResult)
                                            end
                                        elseif (strResult == "ON_DELETE_SET_NULL") then
                                            if (bIsCol == false and bIsForeignKey == true) then
                                                -- it is a foreign key, delete is accepted
                                                if (bOnDeleteCascade) then
                                                    -- Error, using both not compatible delete
                                                    strError = string.format("Table %s - line %d (column='%s'): attribute '%s' using both ON_DELETE_CASCADE and ON_DELETE_SET_NULL is not allowed!",strTableName,iLine,strColumName,strResult)
                                                else
                                                    bOnDeleteSetNull = true
                                                end
                                            else
                                                -- Error
                                                strError = string.format("Table %s - line %d (column='%s'): attribute '%s' not allowed for a DECLARE_COLUMN column, only avalaible for foreign key, use DECLARE_FOREIGN_KEY instead!",strTableName,iLine,strColumName,strResult)
                                            end
                                        elseif (strResult == "ON_UPDATE_CASCADE") then
                                            if (bIsCol == false and bIsForeignKey == true) then
                                                -- it is a foreign key, update is accepted
                                                if (bOnUpdateSetNull) then
                                                    -- Error, using both not compatible update
                                                    strError = string.format("Table %s - line %d (column='%s'): attribute '%s' using both ON_UPDATE_CASCADE and ON_UPDATE_SET_NULL is not allowed!",strTableName,iLine,strColumName,strResult)
                                                else
                                                    bOnUpdateCascade = true
                                                end
                                            else
                                                -- Error
                                                strError = string.format("Table %s - line %d (column='%s'): attribute '%s' not allowed for a DECLARE_COLUMN column, only avalaible for foreign key, use DECLARE_FOREIGN_KEY instead!",strTableName,iLine,strColumName,strResult)
                                            end
                                        elseif (strResult == "ON_UPDATE_SET_NULL") then
                                            if (bIsCol == false and bIsForeignKey == true) then
                                                -- it is a foreign key, update is accepted
                                                if (bOnUpdateCascade) then
                                                    -- Error, using both not compatible update
                                                    strError = string.format("Table %s - line %d (column='%s'): attribute '%s' using both ON_UPDATE_CASCADE and ON_UPDATE_SET_NULL is not allowed!",strTableName,iLine,strColumName,strResult)
                                                else
                                                    bOnUpdateSetNull = true
                                                end
                                            else
                                                -- Error
                                                strError = string.format("Table %s - line %d (column='%s'): attribute '%s' not allowed for a DECLARE_COLUMN column, only avalaible for foreign key, use DECLARE_FOREIGN_KEY instead!",strTableName,iLine,strColumName,strResult)
                                            end
                                        elseif (strResult == "UNIQUE") then
                                            bIsUnique = true
                                            if (bIsPrimary) then
                                                -- Error
                                                strError = string.format("Table %s - line %d (column='%s'): attribute '%s' not allowed for PRIMARY_KEY (always UNIQUE)",strTableName,iLine,strColumName,strResult)
                                            end
                                        elseif (string.sub(strResult,1,1) == "[" and string.sub(strResult,string.len(strResult),string.len(strResult)) == "]") then
                                            -- Init variable into constructor and replace all @ into [] bloc with space
                                            strImplementInitVariableConstructor = string.gsub(string.sub(strResult,2,string.len(strResult) - 1),"@"," ")
                                        else
                                            -- Error
                                            strError = string.format("Table %s - line %d (column='%s'): attribute '%s' is unknown!",strTableName,iLine,strColumName,strResult)
                                        end
                                    end -- function(x) -- Other keywords
                    }
                    iIndexInColums = iIndexInColums + 1
                end -- if (strResult == "$$**$$" or iLine==0) then / else
                if (string.len(strError) ~= 0) then
                    -- If error occurs, break the for
                    break
                end
            end -- for iIndex,strResult in ipairs(tResults) do (split with space)

            -- Check primary key indexes...
            --
            -- If more than one primary key exists and autoincrement is used: an error will be raised on database creation.
            if (string.len(strError) == 0 and (iNbPrimaryKey>=2 and bHasAutoincrementPrimaryKey)) then
                strError = string.format("Table %s contains %d primary key. Using autoincrement for a table that contains more than one primary key is not allowed!",strTableName,iNbPrimaryKey)
            end

            if (string.len(strError) == 0) then
                if (not bUserStructImplementation) then
                    strTemplateHeaderFileCopy   = RemoveAllBlocs("STRUCT_IMPLEMENTATION",strTemplateHeaderFileCopy,true)
                    strTemplateSourceFileCopy   = RemoveAllBlocs("STRUCT_IMPLEMENTATION",strTemplateSourceFileCopy,true)
                else
                    -- No more need REPEAT_VARIABLE, remove it
                    -- This code should be executed all the time but for the moment, no need into normal generation
                    strTemplateHeaderFileCopy   = RemoveAllBlocs("REPEAT_VARIABLE",strTemplateHeaderFileCopy,false)
                    strTemplateSourceFileCopy   = RemoveAllBlocs("REPEAT_VARIABLE",strTemplateSourceFileCopy,false)
                end

                -- Replace header
                strTemplateHeaderFileCopy = string.gsub(strTemplateHeaderFileCopy,"$COMMENT_TABLE%$", function() return dicCommentTables[strTableName] or "" end)
                strTemplateHeaderFileCopy = string.gsub(strTemplateHeaderFileCopy,"$PREFIX_TABLE%$",strPrefixTable)
                strTemplateHeaderFileCopy = string.gsub(strTemplateHeaderFileCopy,"$PREFIX_STRUCT%$",strPrefixStruct)
                strTemplateHeaderFileCopy = string.gsub(strTemplateHeaderFileCopy,"$TABLE_NAME%$",strTableName)
                strTemplateHeaderFileCopy = string.gsub(strTemplateHeaderFileCopy,"$COLUMNS_DECLARATIONS%$",strCPPColumnDeclaration)
                strTemplateHeaderFileCopy = string.gsub(strTemplateHeaderFileCopy,"$COLUMNS_VARIABLES_DECLARATIONS%$",strCPPColumnVariablesDeclaration)
                strTemplateHeaderFileCopy = string.gsub(strTemplateHeaderFileCopy,"$DECLARE_VARIABLES_GETTER_SETTER%$",strHDeclareVariablesGetterSetters)
                strTemplateHeaderFileCopy = string.gsub(strTemplateHeaderFileCopy,"$PREFIX_ARGUMENTS_REFERENCE%$",strPrefixArgumentsReference)
                strTemplateHeaderFileCopy = string.gsub(strTemplateHeaderFileCopy,"$EXPORT_IMPORT%$",strExportImport)

                -- Replace source code
                strTemplateSourceFileCopy = string.gsub(strTemplateSourceFileCopy,"$PREFIX_TABLE%$",strPrefixTable)
                strTemplateSourceFileCopy = string.gsub(strTemplateSourceFileCopy,"$PREFIX_STRUCT%$",strPrefixStruct)
                strTemplateSourceFileCopy = string.gsub(strTemplateSourceFileCopy,"$TABLE_NAME%$",strTableName)
                strTemplateSourceFileCopy = string.gsub(strTemplateSourceFileCopy,"$COLUMNS_IMPLEMENTATION%$",strCPPColumnImplementation)
                strTemplateSourceFileCopy = string.gsub(strTemplateSourceFileCopy,"$COLUMNS_ADD%$",strCPPColumnAdd)
                strTemplateSourceFileCopy = string.gsub(strTemplateSourceFileCopy,"$IMPLEMENT_VARIABLES_GETTER_SETTER%$",strCPPImplementVariablesGetterSetters)
                strTemplateSourceFileCopy = string.gsub(strTemplateSourceFileCopy,"$IMPLEMENT_VARIABLES_CONSTRUCTOR%$",strCPPConstructorVariablesImplementation)
                strTemplateSourceFileCopy = string.gsub(strTemplateSourceFileCopy,"$PREFIX_ARGUMENTS_REFERENCE%$",strPrefixArgumentsReference)
                -- Prepare table construction : Add primary keys if exists
                if (string.len(strCPPTableConstructionPK) ~= 0) then
                    -- Add Primary keys
                    strCPPTableConstruction = strCPPTableConstruction..
                                                string.format("\n%s\"  PRIMARY KEY(%s),\""
                                                              ,string.rep(" ",22 + string.len(strTableName))
                                                              ,strCPPTableConstructionPK
                                                             )
                end

                -- Prepare table construction : remove indentation of first line + remove last ',"'
                strCPPTableConstruction = strCPPTableConstruction .. strCPPTableConstructionFK
                strTemplateSourceFileCopy = string.gsub(strTemplateSourceFileCopy,"$SQL_TABLE_CONSTRUCTION%$",string.sub(strCPPTableConstruction,24 + string.len(strTableName),string.len(strCPPTableConstruction) - 2))
                strTemplateSourceFileCopy = string.gsub(strTemplateSourceFileCopy,"$SQL_TABLE_INDEX_CONSTRUCTION%$", strCPPTableConstructionINDEX)

                -- Add header description before class implementation
                strHContent = strHContent .. string.sub(strWS3Content,iLastExpressionBegin,iExpressionBegin - 1) .. "/*\n"
                                          .. string.sub(strWS3Content,iExpressionBegin,iExpressionEnd) .. "\n*/\n"
                                          .. strTemplateHeaderFileCopy

                -- Add result to content file
                strCPPContent = strCPPContent .. string.sub(strWS3Content,iLastExpressionBegin,iExpressionBegin - 1)
                                              .. strTemplateSourceFileCopy

                iLastExpressionBegin = iExpressionEnd + 1
            else -- if (string.len(strError) ~= 0) then
                -- exit for each, add table description in error
                strCPPContent = strCPPContent .. "/*\n" .. string.sub(strWS3Content,iExpressionBegin,iExpressionEnd) .. "\n*/\n"
                strHContent   = strHContent .. "/*\n" .. string.sub(strWS3Content,iExpressionBegin,iExpressionEnd) .. "\n*/\n"
                break -- exit loop
            end

            -- Extract table to generate for the loop
            iExpressionBegin,iExpressionEnd,strTableName,strColumnDescriptions = string.find(strWS3Content,regExpression,iExpressionEnd)
        end -- while string.len(strError) == 0 and iExpressionBegin ~= nil do

        if (iNbTableFound ~= 0) then
            -- At least one table has been found
            if (string.len(strError) == 0) then
                -- Add end of ws3 file
                strHContent   = strHContent .. string.sub(strWS3Content,iLastExpressionBegin)
                strCPPContent = strCPPContent .. string.sub(strWS3Content,iLastExpressionBegin)
            end
        elseif (string.len(strError) == 0) then
            -- Error, no table where found (only if not another error)
            strError   = string.format("file '%s' doesn't contains any table description!",strFileNameWS3)
        end

        -- Log error
        if (string.len(strError) ~= 0) then
            print(strError)
            strHContent   = strHContent .. "#error \"" .. strError .. "\""    -- <--- Will genrerate a C++ error while compiling
            strCPPContent = strCPPContent .. "#error \"" .. strError .. "\""  -- <--- Will genrerate a C++ error while compiling
            lReturn = -1
        else
            -- Remove all blocs
            -- 1> Remove all these blocs REMOVE_BLOC <-- Always remove this bloc
            strHContent   = RemoveAllBlocs("REMOVE_BLOC",strHContent,false)
            strCPPContent = RemoveAllBlocs("REMOVE_BLOC",strCPPContent,false)

            -- 2> Remove entire blocs
            strHContent   = RemoveAllBlocs("ONLY_SOURCE_FILE",strHContent,false)
            strCPPContent = RemoveAllBlocs("ONLY_HEADER_FILE",strCPPContent,false)

            -- 3> Remove only tags blocs
            strHContent   = RemoveAllBlocs("ONLY_HEADER_FILE",strHContent,true)
            strCPPContent = RemoveAllBlocs("ONLY_SOURCE_FILE",strCPPContent,true)

            -- Replace $FILENAME$ : name of the file
            strHContent   = string.gsub(strHContent,"$FILENAME%$",GetFileName(strFileNameH))
            strCPPContent = string.gsub(strCPPContent,"$FILENAME%$",GetFileName(strFileNameCPP))

            -- Replace $FILENAME_H$ : name of the header file
            strHContent   = string.gsub(strHContent,"$FILENAME_H%$",GetFileName(strFileNameH))
            strCPPContent = string.gsub(strCPPContent,"$FILENAME_H%$",GetFileName(strFileNameH))

            -- Replace $FILENAME_CPP$ : name of the source file
            strHContent   = string.gsub(strHContent,"$FILENAME_CPP%$",GetFileName(strFileNameCPP))
            strCPPContent = string.gsub(strCPPContent,"$FILENAME_CPP%$",GetFileName(strFileNameCPP))

            -- Compute hash key
            if (bGenerateH) then
                strHHash = sha2.hash256(strHContent)
            end
            if (bGenerateCPP) then
                strCPPHash = sha2.hash256(strCPPContent)
            end
            -- Write new Hash keys
            WriteFileContent(RemoveExtension(strFileNameWS3) .. ".hash",strWS3Hash .. "\n" .. strHHash .. "\n" .. strCPPHash)

            if (bGenerateH) then
                print(string.format("Generate %s from %s file...",GetFileName(strFileNameH),GetFileName(strFileNameWS3)))

                -- Write header
                WriteFileContent(strFileNameH,strHContent)
            end
            if (bGenerateCPP) then
                print(string.format("Generate %s from %s file...",GetFileName(strFileNameCPP),GetFileName(strFileNameWS3)))

                -- Write implementation file
                WriteFileContent(strFileNameCPP,strCPPContent)
            end
        end -- if (string.len(strError) ~= 0) then / else
    else -- // elseif (string.len(strError) == 0) then
        print(strError)
        lReturn = -1
    end -- if (not (bGenerateH or bGenerateCPP)) then / elseif / else

end

return lReturn
