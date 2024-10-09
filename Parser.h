#ifndef PARSER_H
#define PARSER_H

    // Start           => JsonValue EOF
    // JsonValue       => Dictionary|Array|Null|Bool|String|Number
    // Dictionary      => { (DictionaryEntry (, DictionaryEntry)*)? }
    // DictionaryEntry => String : JsonValue
    // Array           => [(JsonValue (, JsonValue)*)?]
    // Null            => null
    // Bool            => true|false
    // String          => ".*"
    // Number          => [0-9]+
    // EOF             => $


#endif