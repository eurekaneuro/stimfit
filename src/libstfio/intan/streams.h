#ifndef STREAMS_H
#define STREAMS_H

#include <memory>
#include <string>
#include <cstdint>
#include <iosfwd>
#include <istream>

#if defined(_WIN32) && defined(_UNICODE)
    typedef std::wstring FILENAME;
#else
    typedef std::string FILENAME;
#endif

//  ------------------------------------------------------------------------
class FileOutStream  {
public:
    FileOutStream();
    ~FileOutStream();

    void open(const FILENAME& filename); // Opens with new name
    int write(const char* data, int len);

private:
    std::unique_ptr<std::ofstream> filestream;

    void close();
};

//  ------------------------------------------------------------------------
const unsigned int KILO = 1024;

class BufferedOutStream {
public:
    BufferedOutStream(std::unique_ptr<FileOutStream>&& other_, unsigned int bufferSize_);
    ~BufferedOutStream();

    int write(const char* data, int len);
    void flushIfNecessary();
    void flush();
    
private:
    std::unique_ptr<FileOutStream> other;
    char* dataStreamBuffer;
    unsigned int bufferIndex;
    unsigned int bufferSize;
};

//  ------------------------------------------------------------------------
class BinaryWriter {
public:
    BinaryWriter(std::unique_ptr<FileOutStream>&& other_, unsigned int bufferSize_);
    virtual ~BinaryWriter();

protected:
    friend BinaryWriter& operator<<(BinaryWriter& ostream, int32_t value);
    friend BinaryWriter& operator<<(BinaryWriter& ostream, uint32_t value);
    friend BinaryWriter& operator<<(BinaryWriter& ostream, int16_t value);
    friend BinaryWriter& operator<<(BinaryWriter& ostream, uint16_t value);
    friend BinaryWriter& operator<<(BinaryWriter& ostream, int8_t value);
    friend BinaryWriter& operator<<(BinaryWriter& ostream, uint8_t value);
    friend BinaryWriter& operator<<(BinaryWriter& ostream, float value);
    friend BinaryWriter& operator<<(BinaryWriter& ostream, double value);
    friend BinaryWriter& operator<<(BinaryWriter& ostream, const std::wstring& value);

private:
    BufferedOutStream other;
};

//  ------------------------------------------------------------------------
class InStream {
public:
    virtual ~InStream() {}

    virtual int read(char* data, int len) = 0;
    virtual uint64_t bytesRemaining() = 0;
    virtual std::istream::pos_type currentPos() = 0;
};

//  ------------------------------------------------------------------------
class FileInStream : public InStream {
public:
    FileInStream();
    ~FileInStream();

    virtual bool open(const FILENAME& filename); // Opens with new name
    virtual int read(char* data, int len) override;
    uint64_t bytesRemaining() override;
    std::istream::pos_type currentPos() override;

private:
    std::unique_ptr<std::ifstream> filestream;
    std::istream::pos_type filesize;

    virtual void close();
};

//  ------------------------------------------------------------------------
class BinaryReader {
public:
    BinaryReader(std::unique_ptr<FileInStream>&& other_);
    virtual ~BinaryReader();

    uint64_t bytesRemaining() { return other->bytesRemaining();  }
    std::istream::pos_type currentPos() { return other->currentPos(); }

protected:
    friend BinaryReader& operator>>(BinaryReader& istream, int32_t& value);
    friend BinaryReader& operator>>(BinaryReader& istream, uint32_t& value);
    friend BinaryReader& operator>>(BinaryReader& istream, int16_t& value);
    friend BinaryReader& operator>>(BinaryReader& istream, uint16_t& value);
    friend BinaryReader& operator>>(BinaryReader& istream, int8_t& value);
    friend BinaryReader& operator>>(BinaryReader& istream, uint8_t& value);
    friend BinaryReader& operator>>(BinaryReader& istream, float& value);
    friend BinaryReader& operator>>(BinaryReader& istream, double& value);
    friend BinaryReader& operator>>(BinaryReader& istream, std::wstring& value);

private:
    std::unique_ptr<FileInStream> other;
};

FILENAME toFileName(const std::string& s);
FILENAME toFileName(const std::wstring& ws);

#endif // STREAMS_H
