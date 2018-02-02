#pragma once 
#include <cstdint>
#include <ostream>

namespace swizzle { namespace parser {

    enum class ParserState : std::uint8_t {
        Init,
        StartNamespace,
        NamespaceValue,
        NamespaceFirstColon,
        TranslationUnitMain,
        StartImport,
        ImportValue,
        ImportFirstColon,
        StartExtern,
        ExternValue,
        ExternFirstColon,
        StartUsing,
        UsingName,
        UsingReadEqual,
        UsingTypeRead,
        UsingFirstColon,
        UsingSecondColon,
        StartEnum,
        EnumName,
        EnumColonRead,
        EnumUnderlyingType,
        EnumStartScope,
        EnumField,
        EnumFieldEqualRead,
        EnumFieldValueRead,
        StartBitfield,
        BitfieldName,
        BitfieldColonRead,
        BitfieldUnderlyingType,
        BitfieldStartScope,
        BitfieldField,
        BitfieldFieldColonRead,
        BitfieldStartPosition,
        BitfieldFirstDot,
        BitfieldSecondDot,
        BitfieldEndPosition,
        StartStruct,
        StructName,
        StructStartScope,
        StructFieldEnumOrNamespaceRead,
        StructFieldEnumOrNamespaceFirstColonRead,
        StructFieldEnumValueRead,
        StructFieldLabel,
        StructFieldNamespaceOrType,
        StructFieldNamespaceFirstColon,
        StructFieldNamespaceSecondColon,
        StructFieldName,
        StructFieldEqualRead,
        StructFieldValueRead,
        StructStartArray,
        StructArray,
        StructVector,
        StructVectorNestedOnMember,
        StructEndArrayOrVector,
        StructStartVariableBlock,
        StructVariableBlockColonRead,
        StructVariableBlockOnField,
        StructVariableBlockOnNestedField,
        StructVariableBlockBeginCases,
        StructVariableBlockCaseValue,
        StructVariableBlockCaseValueRead,
        StructVariableBlockCaseValueColonRead,
        StructVariableBlockCaseBlockNameRead,
        StructVariableBlockNamespaceFirstColonRead,
        StructVariableBlockNamespaceSecondColonRead,
    };

    std::ostream& operator<<(std::ostream& os, const ParserState state);
}}
