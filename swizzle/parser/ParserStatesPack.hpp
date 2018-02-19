#pragma once 

#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/ParserState.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/types/NodeStack.hpp>
#include <swizzle/types/TokenStack.hpp>

#include <swizzle/parser/states/BitfieldColonReadState.hpp>
#include <swizzle/parser/states/BitfieldEndPositionState.hpp>
#include <swizzle/parser/states/BitfieldFieldColonReadState.hpp>
#include <swizzle/parser/states/BitfieldFieldState.hpp>
#include <swizzle/parser/states/BitfieldFirstDotState.hpp>
#include <swizzle/parser/states/BitfieldNameState.hpp>
#include <swizzle/parser/states/BitfieldSecondDotState.hpp>
#include <swizzle/parser/states/BitfieldStartPositionState.hpp>
#include <swizzle/parser/states/BitfieldStartScopeState.hpp>
#include <swizzle/parser/states/BitfieldUnderlyingTypeState.hpp>
#include <swizzle/parser/states/EnumColonReadState.hpp>
#include <swizzle/parser/states/EnumFieldEqualReadState.hpp>
#include <swizzle/parser/states/EnumFieldState.hpp>
#include <swizzle/parser/states/EnumFieldValueReadState.hpp>
#include <swizzle/parser/states/EnumNameState.hpp>
#include <swizzle/parser/states/EnumStartScopeState.hpp>
#include <swizzle/parser/states/EnumUnderlyingTypeState.hpp>
#include <swizzle/parser/states/ExternFirstColonState.hpp>
#include <swizzle/parser/states/ExternValueState.hpp>
#include <swizzle/parser/states/ImportFirstColonState.hpp>
#include <swizzle/parser/states/ImportValueState.hpp>
#include <swizzle/parser/states/InitState.hpp>
#include <swizzle/parser/states/NamespaceFirstColonState.hpp>
#include <swizzle/parser/states/NamespaceValueState.hpp>
#include <swizzle/parser/states/StartBitfieldState.hpp>
#include <swizzle/parser/states/StartEnumState.hpp>
#include <swizzle/parser/states/StartExternState.hpp>
#include <swizzle/parser/states/StartImportState.hpp>
#include <swizzle/parser/states/StartNamespaceState.hpp>
#include <swizzle/parser/states/StartStructState.hpp>
#include <swizzle/parser/states/StartUsingState.hpp>
#include <swizzle/parser/states/StructArrayState.hpp>
#include <swizzle/parser/states/StructBaseColonReadState.hpp>
#include <swizzle/parser/states/StructBaseNameOrNamespaceFirstColonReadState.hpp>
#include <swizzle/parser/states/StructBaseNameOrNamespaceReadState.hpp>
#include <swizzle/parser/states/StructEndArrayOrVectorState.hpp>
#include <swizzle/parser/states/StructFieldEnumOrNamespaceFirstColonReadState.hpp>
#include <swizzle/parser/states/StructFieldEnumOrNamespaceReadState.hpp>
#include <swizzle/parser/states/StructFieldEnumValueReadState.hpp>
#include <swizzle/parser/states/StructFieldEqualReadState.hpp>
#include <swizzle/parser/states/StructFieldLabelState.hpp>
#include <swizzle/parser/states/StructFieldNameState.hpp>
#include <swizzle/parser/states/StructFieldNamespaceFirstColonState.hpp>
#include <swizzle/parser/states/StructFieldNamespaceOrTypeState.hpp>
#include <swizzle/parser/states/StructFieldNamespaceSecondColonState.hpp>
#include <swizzle/parser/states/StructFieldValueReadState.hpp>
#include <swizzle/parser/states/StructNameState.hpp>
#include <swizzle/parser/states/StructStartArrayState.hpp>
#include <swizzle/parser/states/StructStartScopeState.hpp>
#include <swizzle/parser/states/StructStartVariableBlockState.hpp>
#include <swizzle/parser/states/StructVariableBlockBeginCasesState.hpp>
#include <swizzle/parser/states/StructVariableBlockCaseBlockNameReadState.hpp>
#include <swizzle/parser/states/StructVariableBlockCaseValueColonReadState.hpp>
#include <swizzle/parser/states/StructVariableBlockCaseValueReadState.hpp>
#include <swizzle/parser/states/StructVariableBlockCaseValueState.hpp>
#include <swizzle/parser/states/StructVariableBlockColonReadState.hpp>
#include <swizzle/parser/states/StructVariableBlockNamespaceFirstColonReadState.hpp>
#include <swizzle/parser/states/StructVariableBlockNamespaceSecondColonReadState.hpp>
#include <swizzle/parser/states/StructVariableBlockOnFieldState.hpp>
#include <swizzle/parser/states/StructVariableBlockOnNestedFieldState.hpp>
#include <swizzle/parser/states/StructVectorNestedOnMemberState.hpp>
#include <swizzle/parser/states/StructVectorState.hpp>
#include <swizzle/parser/states/TranslationUnitMainState.hpp>
#include <swizzle/parser/states/UsingFirstColonState.hpp>
#include <swizzle/parser/states/UsingNameState.hpp>
#include <swizzle/parser/states/UsingReadEqualState.hpp>
#include <swizzle/parser/states/UsingSecondColonState.hpp>
#include <swizzle/parser/states/UsingTypeReadState.hpp>

namespace swizzle { namespace parser {

    class ParserStatesPack
    {
    public:
        ParserState consume(const ParserState state, const lexer::TokenInfo& token, types::NodeStack& nodeStack, types::NodeStack& attributeStack, types::TokenStack& tokenStack, ParserStateContext& context);

    private:

        states::InitState initState_;
        states::StartNamespaceState startNamespaceState_;
        states::NamespaceValueState namespaceValueState_;
        states::NamespaceFirstColonState namespaceFirstColonState_;
        states::TranslationUnitMainState translationUnitMainState_;
        states::StartImportState startImportState_;
        states::ImportValueState importValueState_;
        states::ImportFirstColonState importFirstColonState_;
        states::StartExternState startExternState_;
        states::ExternValueState externValueState_;
        states::ExternFirstColonState externFirstColonState_;
        states::StartUsingState startUsingState_;
        states::UsingNameState usingNameState_;
        states::UsingReadEqualState usingReadEqualState_;
        states::UsingTypeReadState usingTypeReadState_;
        states::UsingFirstColonState usingFirstColonState_;
        states::UsingSecondColonState usingSecondColonState_;
        states::StartEnumState startEnumState_;
        states::EnumNameState enumNameState_;
        states::EnumColonReadState enumColonReadState_;
        states::EnumUnderlyingTypeState enumUnderlyingTypeState_;
        states::EnumStartScopeState enumStartScopeState_;
        states::EnumFieldState enumFieldState_;
        states::EnumFieldEqualReadState enumFieldEualReadState_;
        states::EnumFieldValueReadState enumFieldValueReadState_;
        states::StartBitfieldState startBitfieldState_;
        states::BitfieldNameState bitfieldNameState_;
        states::BitfieldColonReadState bitfieldColonReadState_;
        states::BitfieldUnderlyingTypeState bitfieldUnderlyingTypeState_;
        states::BitfieldStartScopeState bitfieldStartScopeState_;
        states::BitfieldFieldState bitfieldFieldState_;
        states::BitfieldFieldColonReadState bitfieldFieldColonReadState_;
        states::BitfieldStartPositionState bitfieldStartPositionState_;
        states::BitfieldFirstDotState bitfieldFirstDotState_;
        states::BitfieldSecondDotState bitfieldSecondDotState_;
        states::BitfieldEndPositionState bitfieldEndPositionState_;
        states::StructBaseColonReadState structBaseColonReadState_;
        states::StructBaseNameOrNamespaceFirstColonReadState structBaseNameOrNamespaceReadState_;
        states::StructBaseNameOrNamespaceReadState structBaseNameOrNamespaceReadState_;
        states::StartStructState startStructState_;
        states::StructNameState structNameState_;
        states::StructStartScopeState structStartScopeState_;
        states::StructFieldLabelState structFieldLabelState_;
        states::StructFieldNamespaceOrTypeState structFieldNamespaceOrTypeState_;
        states::StructFieldNamespaceFirstColonState structFieldNamespaceFirstColonState_;
        states::StructFieldNamespaceSecondColonState structFieldNamespaceSecondColonState_;
        states::StructFieldNameState structFieldNameState_;
        states::StructFieldEnumOrNamespaceFirstColonReadState structFieldEnumOrNamespaceFirstColonReadState_;
        states::StructFieldEnumOrNamespaceReadState structFieldEnumOrNamespaceReadState_;
        states::StructFieldEnumValueReadState structFieldEnumValueReadState_;
        states::StructFieldEqualReadState structFieldEqualReadState_;
        states::StructFieldValueReadState structFieldValueReadState_;
        states::StructStartArrayState structStartArrayState_;
        states::StructArrayState structArrayState_;
        states::StructVectorState structVectorState_;
        states::StructVectorNestedOnMemberState structVectorNestedOnMemberState_;
        states::StructEndArrayOrVectorState structEndArrayOrVectorState_;
        states::StructStartVariableBlockState structStartVariableBlockState_;
        states::StructVariableBlockColonReadState structVariableBlockColonReadState_;
        states::StructVariableBlockOnFieldState structVariableBlockOnFieldState_;
        states::StructVariableBlockOnNestedFieldState structVariableBlockOnNestedFieldState_;
        states::StructVariableBlockBeginCasesState structVariableBlockBeginCasesState_;
        states::StructVariableBlockCaseValueState structVariableBlockCaseValueState_;
        states::StructVariableBlockCaseValueReadState structVariableBlockCaseValueReadState_;
        states::StructVariableBlockCaseValueColonReadState structVariableBlockCaseValueColonReadState_;
        states::StructVariableBlockCaseBlockNameReadState structVariableBlockCaseBlockNameReadState_;
        states::StructVariableBlockNamespaceFirstColonReadState structVariableBlockNamespaceFirstColonReadState_;
        states::StructVariableBlockNamespaceSecondColonReadState structVariableBlockNamespaceSecondColonReadState_;
    };
}}
