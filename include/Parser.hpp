#pragma once

#include "PrivateTypes.hpp"
#include "TokenExtractor.hpp"
#include "ErrorsWriter.hpp"

namespace Lazynput
{
    /// \class Parser
    /// \brief Parses data an input stream.
    class Parser
    {
        private:
            /// Writes parsing errors to the errors stream.
            ErrorsWriter errorsWriter;

            /// Extracts tokens from the input stream.
            TokenExtractor extractor;

            /// Temporary container for new devices database. Is discarded on any error encountered.
            DevicesDb newDevicesDb;

            /// Old devices database to check for redefinitions and to use previously parsed labels presets.
            DevicesDb &oldDevicesDb;

            /// \brief Switch to the next state if the next token is the expected token.
            ///
            /// Check if the next token is an opening brace or a new line.
            /// Updates the state if the next token
            /// Generates an error if the next token is not the expected token nor a newline.
            ///
            /// \param state : will be updated if the conditions are met.
            /// \param hash : next token's hash.
            /// \param expectedHash : expected token's hash.
            /// \param token : next token.
            /// \param nextState : updated value of state if the conditions are met.
            ///
            /// \return : true if there are not errors, false orherwise
            bool expectToken(uint8_t *state, StrHash hash, StrHash expectedHash, const std::string &token,
                uint8_t nextState);

            /// \brief Parse an interfaces block.
            ///
            /// Parse interfaces data from a text input stream. Conflicting interface declarations will generate errors.
            ///
            /// \return true if successfully parsed, false otherwise.
            bool parseInterfacesBlock();

            /// \brief Parse a labels block from input stream.
            ///
            /// Parse labels data from a text input stream. Existing data will be overrided and applied to all devices,
            /// boarh already parsed and parsed afterwards.
            ///
            /// \return true if successfully parsed, false otherwise.
            bool parseLabelsBlock();

            /// \brief Parses a devices block from input stream.
            ///
            /// Parse devices data from a text input stream. Existing data will be overrided.
            ///
            /// \return true if successfully parsed, false otherwise.
            bool parseDevicesBlock();

            /// \brief Parses a labels definition block.
            /// \param interfaces : sorted vector containing interfaces to look for, or nullptr for a preset definition.
            /// \param labels : hashmap to be filled. Can be modified even if the parsing fails.
            /// \return true if successfully parsed, false otherwise.
            bool parseLabelsBlock(const std::vector<StrHash> *interfaces, StrHashMap<LabelInfosPrivate> &labels);

        public:
            /// \brief Constructs and initializes the parser.
            /// \param inStream : a istream providing the text input to be parsed.
            /// \param errors : a stream to write parsing errors, if any. Can be null.
            /// \param interfacesDb : interfaces database to be updated if the stream is successfully parsed.
            Parser(std::istream &inStream, std::ostream *errors, DevicesDb &devicesDb);

            /// \brief Parses the input stream
            ///
            /// Parses the input stream and updates the databases if the stream is successfully parsed.
            /// On any error encountered, every new definitions are discarded and the databases will be unchanged.
            ///
            /// \return true if successfully parsed, false otherwise.
            bool parse();

    };
}
