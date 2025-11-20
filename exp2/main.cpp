#include <iostream>
#include <string>
#include "Huffman.h"

using namespace std;

// 马丁·路德·金《I Have a Dream》演讲片段
const string DREAM_SPEECH = 
    "i have a dream that one day this nation will rise up and live out the true meaning of its creed we hold these truths to be self evident that all men are created equal "
    "i have a dream that one day on the red hills of georgia the sons of former slaves and the sons of former slave owners will be able to sit down together at the table of brotherhood "
    "i have a dream that one day even the state of mississippi a state sweltering with the heat of injustice sweltering with the heat of oppression will be transformed into an oasis of freedom and justice "
    "i have a dream that my four little children will one day live in a nation where they will not be judged by the color of their skin but by the content of their character "
    "i have a dream today "
    "i have a dream that one day down in alabama with its vicious racists with its governor having his lips dripping with the words of interposition and nullification one day right down in alabama little black boys and black girls will be able to join hands with little white boys and white girls as sisters and brothers "
    "i have a dream today "
    "i have a dream that one day every valley shall be exalted every hill and mountain shall be made low the rough places will be made plain and the crooked places will be made straight and the glory of the lord shall be revealed and all flesh shall see it together";

void printSeparator() {
    cout << "=========================================" << endl;
}

void testHuffmanEncoding() {
    cout << "=== Huffman Encoding Test ===" << endl;
    cout << "Using Martin Luther King's 'I Have a Dream' speech" << endl;
    printSeparator();
    
    // 分析文本
    Vector<char> characters;
    Vector<int> frequencies;
    TextAnalyzer::analyzeText(DREAM_SPEECH, characters, frequencies);
    
    cout << "Character frequencies found in speech:" << endl;
    for (int i = 0; i < characters.size(); i++) {
        cout << characters[i] << ": " << frequencies[i] << "  ";
        if ((i + 1) % 8 == 0) cout << endl;
    }
    cout << endl << endl;
    
    // 构建 Huffman 树
    HuffmanTree<char> huffmanTree;
    huffmanTree.build(characters, frequencies);
    
    // 显示编码表
    huffmanTree.displayCodeTable();
    cout << endl;
    
    // 测试编码
    Vector<string> testWords;
    testWords.push_back("dream");
    testWords.push_back("freedom");
    testWords.push_back("justice");
    testWords.push_back("brotherhood");
    testWords.push_back("equality");
    testWords.push_back("hope");
    
    for (int i = 0; i < testWords.size(); i++) {
        const string& word = testWords[i];
        string encoded = huffmanTree.encode(word);
        string decoded = huffmanTree.decode(encoded);
        
        printSeparator();
        cout << "Test Word " << (i + 1) << ": " << word << endl;
        cout << "Encoded: " << encoded << endl;
        cout << "Decoded: " << decoded << endl;
        cout << "Original length: " << word.length() * 8 << " bits" << endl;
        cout << "Encoded length: " << encoded.length() << " bits" << endl;
        double ratio = (double)encoded.length() / (word.length() * 8) * 100;
        cout << "Compression ratio: " << ratio << "%" << endl;
        cout << "Encoding correct: " << (word == decoded ? "YES" : "NO") << endl;
    }
    
    // 测试整个短语
    printSeparator();
    string testPhrase = "ihaveadream";
    string encodedPhrase = huffmanTree.encode(testPhrase);
    string decodedPhrase = huffmanTree.decode(encodedPhrase);
    
    cout << "Test Phrase: " << testPhrase << endl;
    cout << "Encoded: " << encodedPhrase << endl;
    cout << "Decoded: " << decodedPhrase << endl;
    cout << "Original length: " << testPhrase.length() * 8 << " bits" << endl;
    cout << "Encoded length: " << encodedPhrase.length() << " bits" << endl;
    double ratio = (double)encodedPhrase.length() / (testPhrase.length() * 8) * 100;
    cout << "Compression ratio: " << ratio << "%" << endl;
    cout << "Encoding correct: " << (testPhrase == decodedPhrase ? "YES" : "NO") << endl;
    
    printSeparator();
    cout << "Huffman Encoding Test Completed!" << endl;
}

int main() {
    testHuffmanEncoding();
    return 0;
}
