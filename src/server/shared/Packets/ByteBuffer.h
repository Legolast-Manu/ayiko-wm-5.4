/*
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _BYTEBUFFER_H
#define _BYTEBUFFER_H

#include "Define.h"
#include "Errors.h"
#include "ByteConverter.h"
#include "ObjectGuid.h"

#include <list>
#include <map>
#include <string>
#include <type_traits>
#include <vector>

// Root of ByteBuffer exception hierarchy
class ByteBufferException : public std::exception
{
public:
    ~ByteBufferException() throw() { }

    char const* what() const throw() final { return msg_.c_str(); }

protected:
    std::string & message() throw() { return msg_; }

private:
    std::string msg_;
};

class ByteBufferPositionException final : public ByteBufferException
{
public:
    ByteBufferPositionException(bool add, size_t pos, size_t size, size_t valueSize);

    ~ByteBufferPositionException() throw() { }
};

class ByteBufferSourceException final : public ByteBufferException
{
public:
    ByteBufferSourceException(size_t pos, size_t size, size_t valueSize);

    ~ByteBufferSourceException() throw() { }
};

class ByteBuffer
{
    public:
        const static size_t DEFAULT_SIZE = 0x1000;

        // constructor
        ByteBuffer() : _rpos(0), _wpos(0), _bitpos(8), _curbitval(0)
        {
            _storage.reserve(DEFAULT_SIZE);
        }

        ByteBuffer(size_t reserve) : _rpos(0), _wpos(0), _bitpos(8), _curbitval(0)
        {
            _storage.reserve(reserve);
        }

        // copy constructor
        ByteBuffer(const ByteBuffer &buf) : _rpos(buf._rpos), _wpos(buf._wpos),
            _bitpos(buf._bitpos), _curbitval(buf._curbitval), _storage(buf._storage)
        {
        }

        void clear()
        {
            _storage.clear();
            _rpos = _wpos = 0;
            _bitpos = 8;
            _curbitval = 0;
        }

        template <typename T>
        typename std::enable_if<std::is_arithmetic<T>::value>::type
        append(T value)
        {
            FlushBits();
            EndianConvert(value);
            append((uint8 const *)&value, sizeof(value));
        }

        template <typename T>
        typename std::enable_if<std::is_enum<T>::value>::type
        append(T value)
        {
            typedef typename std::underlying_type<T>::type UnderlyingType;
            append(static_cast<UnderlyingType>(value));
        }

        void FlushBits()
        {
            if (_bitpos == 8)
                return;

            append(&_curbitval, sizeof(uint8));
            _curbitval = 0;
            _bitpos = 8;
        }

        template <typename T>
        typename std::enable_if<std::is_arithmetic<T>::value, bool>::type
        WriteBit(T bit)
        {
            bool const nonZero = bit != T();

            --_bitpos;

            if (nonZero)
                _curbitval |= (1 << (_bitpos));

            if (_bitpos == 0)
            {
                _bitpos = 8;
                append((uint8 *)&_curbitval, sizeof(_curbitval));
                _curbitval = 0;
            }

            return nonZero;
        }

        template <typename T>
        typename std::enable_if<std::is_arithmetic<T>::value>::type
        WriteBits(T value, size_t bits)
        {
            for (int32 i = bits-1; i >= 0; --i)
                WriteBit((value >> i) & 1);
        }

        template <typename T>
        typename std::enable_if<std::is_enum<T>::value>::type
        WriteBits(T value, size_t bits)
        {
            typedef typename std::underlying_type<T>::type UnderlyingType;
            WriteBits(static_cast<UnderlyingType>(value), bits);
        }

        template <std::size_t B1>
        void WriteBitSeq(ObjectGuid guid)
        {
            static_assert(B1 < sizeof(uint64_t), "Illegal ObjectGuid bit");
            WriteBit(guid[B1]);
        }

        template <std::size_t B1, std::size_t B2>
        void WriteBitSeq(ObjectGuid guid)
        {
            WriteBitSeq<B1>(guid);
            WriteBitSeq<B2>(guid);
        }

        template <std::size_t B1, std::size_t B2, std::size_t B3>
        void WriteBitSeq(ObjectGuid guid)
        {
            WriteBitSeq<B1, B2>(guid);
            WriteBitSeq<B3>(guid);
        }

        template <std::size_t B1, std::size_t B2, std::size_t B3, std::size_t B4>
        void WriteBitSeq(ObjectGuid guid)
        {
            WriteBitSeq<B1, B2, B3>(guid);
            WriteBitSeq<B4>(guid);
        }

        template <
            std::size_t B1, std::size_t B2, std::size_t B3, std::size_t B4,
            std::size_t B5
        >
        void WriteBitSeq(ObjectGuid guid)
        {
            WriteBitSeq<B1, B2, B3, B4>(guid);
            WriteBitSeq<B5>(guid);
        }

        template <
            std::size_t B1, std::size_t B2, std::size_t B3, std::size_t B4,
            std::size_t B5, std::size_t B6
        >
        void WriteBitSeq(ObjectGuid guid)
        {
            WriteBitSeq<B1, B2, B3, B4, B5>(guid);
            WriteBitSeq<B6>(guid);
        }

        template <
            std::size_t B1, std::size_t B2, std::size_t B3, std::size_t B4,
            std::size_t B5, std::size_t B6, std::size_t B7
        >
        void WriteBitSeq(ObjectGuid guid)
        {
            WriteBitSeq<B1, B2, B3, B4, B5, B6>(guid);
            WriteBitSeq<B7>(guid);
        }

        template <
            std::size_t B1, std::size_t B2, std::size_t B3, std::size_t B4,
            std::size_t B5, std::size_t B6, std::size_t B7, std::size_t B8
        >
        void WriteBitSeq(ObjectGuid guid)
        {
            WriteBitSeq<B1, B2, B3, B4, B5, B6, B7>(guid);
            WriteBitSeq<B8>(guid);
        }

        void WriteByte(uint8 byte)
        {
            if (byte != 0)
                append<uint8>(byte ^ 1);
        }

        template <std::size_t B1>
        void WriteByteSeq(ObjectGuid guid)
        {
            static_assert(B1 < sizeof(uint64_t), "Illegal ObjectGuid bit");
            WriteByte(guid[B1]);
        }

        template <std::size_t B1, std::size_t B2>
        void WriteByteSeq(ObjectGuid guid)
        {
            WriteByteSeq<B1>(guid);
            WriteByteSeq<B2>(guid);
        }

        template <std::size_t B1, std::size_t B2, std::size_t B3>
        void WriteByteSeq(ObjectGuid guid)
        {
            WriteByteSeq<B1, B2>(guid);
            WriteByteSeq<B3>(guid);
        }

        template <std::size_t B1, std::size_t B2, std::size_t B3, std::size_t B4>
        void WriteByteSeq(ObjectGuid guid)
        {
            WriteByteSeq<B1, B2, B3>(guid);
            WriteByteSeq<B4>(guid);
        }

        template <
            std::size_t B1, std::size_t B2, std::size_t B3, std::size_t B4,
            std::size_t B5
        >
        void WriteByteSeq(ObjectGuid guid)
        {
            WriteByteSeq<B1, B2, B3, B4>(guid);
            WriteByteSeq<B5>(guid);
        }

        template <
            std::size_t B1, std::size_t B2, std::size_t B3, std::size_t B4,
            std::size_t B5, std::size_t B6
        >
        void WriteByteSeq(ObjectGuid guid)
        {
            WriteByteSeq<B1, B2, B3, B4, B5>(guid);
            WriteByteSeq<B6>(guid);
        }

        template <
            std::size_t B1, std::size_t B2, std::size_t B3, std::size_t B4,
            std::size_t B5, std::size_t B6, std::size_t B7
        >
        void WriteByteSeq(ObjectGuid guid)
        {
            WriteByteSeq<B1, B2, B3, B4, B5, B6>(guid);
            WriteByteSeq<B7>(guid);
        }

        template <
            std::size_t B1, std::size_t B2, std::size_t B3, std::size_t B4,
            std::size_t B5, std::size_t B6, std::size_t B7, std::size_t B8
        >
        void WriteByteSeq(ObjectGuid guid)
        {
            WriteByteSeq<B1, B2, B3, B4, B5, B6, B7>(guid);
            WriteByteSeq<B8>(guid);
        }

        bool ReadBit()
        {
            ++_bitpos;
            if (_bitpos > 7)
            {
                _bitpos = 0;
                _curbitval = read<uint8>();
            }

            return ((_curbitval >> (7-_bitpos)) & 1) != 0;
        }

        uint32 ReadBits(size_t bits)
        {
            uint32 value = 0;
            for (int32 i = bits-1; i >= 0; --i)
                if (ReadBit())
                    value |= (1 << (i));

            return value;
        }

        template <std::size_t B1>
        void ReadBitSeq(ObjectGuid &guid)
        {
            static_assert(B1 < sizeof(uint64_t), "Illegal ObjectGuid bit");
            guid[B1] = ReadBit();
        }

        template <std::size_t B1, std::size_t B2>
        void ReadBitSeq(ObjectGuid &guid)
        {
            ReadBitSeq<B1>(guid);
            ReadBitSeq<B2>(guid);
        }

        template <std::size_t B1, std::size_t B2, std::size_t B3>
        void ReadBitSeq(ObjectGuid &guid)
        {
            ReadBitSeq<B1, B2>(guid);
            ReadBitSeq<B3>(guid);
        }

        template <std::size_t B1, std::size_t B2, std::size_t B3, std::size_t B4>
        void ReadBitSeq(ObjectGuid &guid)
        {
            ReadBitSeq<B1, B2, B3>(guid);
            ReadBitSeq<B4>(guid);
        }

        template <
            std::size_t B1, std::size_t B2, std::size_t B3, std::size_t B4,
            std::size_t B5
        >
        void ReadBitSeq(ObjectGuid &guid)
        {
            ReadBitSeq<B1, B2, B3, B4>(guid);
            ReadBitSeq<B5>(guid);
        }

        template <
            std::size_t B1, std::size_t B2, std::size_t B3, std::size_t B4,
            std::size_t B5, std::size_t B6
        >
        void ReadBitSeq(ObjectGuid &guid)
        {
            ReadBitSeq<B1, B2, B3, B4, B5>(guid);
            ReadBitSeq<B6>(guid);
        }

        template <
            std::size_t B1, std::size_t B2, std::size_t B3, std::size_t B4,
            std::size_t B5, std::size_t B6, std::size_t B7
        >
        void ReadBitSeq(ObjectGuid &guid)
        {
            ReadBitSeq<B1, B2, B3, B4, B5, B6>(guid);
            ReadBitSeq<B7>(guid);
        }

        template <
            std::size_t B1, std::size_t B2, std::size_t B3, std::size_t B4,
            std::size_t B5, std::size_t B6, std::size_t B7, std::size_t B8
        >
        void ReadBitSeq(ObjectGuid &guid)
        {
            ReadBitSeq<B1, B2, B3, B4, B5, B6, B7>(guid);
            ReadBitSeq<B8>(guid);
        }

        void ReadByte(uint8 &byte)
        {
            if (byte != 0)
                byte ^= read<uint8>();
        }

        template <std::size_t B1>
        void ReadByteSeq(ObjectGuid &guid)
        {
            static_assert(B1 < sizeof(uint64_t), "Illegal ObjectGuid bit");
            ReadByte(guid[B1]);
        }

        template <std::size_t B1, std::size_t B2>
        void ReadByteSeq(ObjectGuid &guid)
        {
            ReadByteSeq<B1>(guid);
            ReadByteSeq<B2>(guid);
        }

        template <std::size_t B1, std::size_t B2, std::size_t B3>
        void ReadByteSeq(ObjectGuid &guid)
        {
            ReadByteSeq<B1, B2>(guid);
            ReadByteSeq<B3>(guid);
        }

        template <std::size_t B1, std::size_t B2, std::size_t B3, std::size_t B4>
        void ReadByteSeq(ObjectGuid &guid)
        {
            ReadByteSeq<B1, B2, B3>(guid);
            ReadByteSeq<B4>(guid);
        }

        template <
            std::size_t B1, std::size_t B2, std::size_t B3, std::size_t B4,
            std::size_t B5
        >
        void ReadByteSeq(ObjectGuid &guid)
        {
            ReadByteSeq<B1, B2, B3, B4>(guid);
            ReadByteSeq<B5>(guid);
        }

        template <
            std::size_t B1, std::size_t B2, std::size_t B3, std::size_t B4,
            std::size_t B5, std::size_t B6
        >
        void ReadByteSeq(ObjectGuid &guid)
        {
            ReadByteSeq<B1, B2, B3, B4, B5>(guid);
            ReadByteSeq<B6>(guid);
        }

        template <
            std::size_t B1, std::size_t B2, std::size_t B3, std::size_t B4,
            std::size_t B5, std::size_t B6, std::size_t B7
        >
        void ReadByteSeq(ObjectGuid &guid)
        {
            ReadByteSeq<B1, B2, B3, B4, B5, B6>(guid);
            ReadByteSeq<B7>(guid);
        }

        template <
            std::size_t B1, std::size_t B2, std::size_t B3, std::size_t B4,
            std::size_t B5, std::size_t B6, std::size_t B7, std::size_t B8
        >
        void ReadByteSeq(ObjectGuid &guid)
        {
            ReadByteSeq<B1, B2, B3, B4, B5, B6, B7>(guid);
            ReadByteSeq<B8>(guid);
        }

        template <typename T>
        typename std::enable_if<std::is_arithmetic<T>::value>::type
        put(size_t pos, T value)
        {
            EndianConvert(value);
            put(pos, (uint8 const *)&value, sizeof(value));
        }

        template <typename T>
        typename std::enable_if<std::is_enum<T>::value>::type
        put(size_t pos, T value)
        {
            typedef typename std::underlying_type<T>::type UnderlyingType;
            put(pos, static_cast<UnderlyingType>(value));
        }

        /**
          * @name   PutBits
          * @brief  Places specified amount of bits of value at specified position in packet.
          *         To ensure all bits are correctly written, only call this method after
          *         bit flush has been performed

          * @param  pos Position to place the value at, in bits. The entire value must fit in the packet
          *             It is advised to obtain the position using bitwpos() function.

          * @param  value Data to write.
          * @param  bitCount Number of bits to store the value on.
        */
        template <typename T>
        typename std::enable_if<std::is_arithmetic<T>::value>::type
        PutBits(size_t pos, T value, uint32 bitCount)
        {
            if (!bitCount)
                throw ByteBufferSourceException((pos + bitCount) / 8, size(), 0);

            if (pos + bitCount > size() * 8 + (8 - _bitpos))
                throw ByteBufferPositionException(false, (pos + bitCount) / 8, size(), (bitCount - 1) / 8 + 1);

            for (uint32 i = 0; i < bitCount; ++i)
            {
                size_t const wp = (pos + i) / 8;
                size_t const bit = (pos + i) % 8;

                uint8 &dest = (wp < size()) ? _storage[wp] : _curbitval;

                if ((value >> (bitCount - i - 1)) & 1)
                    dest |= 1 << (7 - bit);
                else
                    dest &= ~(1 << (7 - bit));
            }
        }

        template <typename T>
        typename std::enable_if<std::is_enum<T>::value>::type
        PutBits(size_t pos, T value, uint32 bitCount)
        {
            typedef typename std::underlying_type<T>::type UnderlyingType;
            PutBits(pos, static_cast<UnderlyingType>(value), bitCount);
        }

        ByteBuffer &operator<<(uint8 value)
        {
            append<uint8>(value);
            return *this;
        }

        ByteBuffer &operator<<(uint16 value)
        {
            append<uint16>(value);
            return *this;
        }

        ByteBuffer &operator<<(uint32 value)
        {
            append<uint32>(value);
            return *this;
        }

        ByteBuffer &operator<<(uint64 value)
        {
            append<uint64>(value);
            return *this;
        }

        // signed as in 2e complement
        ByteBuffer &operator<<(int8 value)
        {
            append<int8>(value);
            return *this;
        }

        ByteBuffer &operator<<(int16 value)
        {
            append<int16>(value);
            return *this;
        }

        ByteBuffer &operator<<(int32 value)
        {
            append<int32>(value);
            return *this;
        }

        ByteBuffer &operator<<(int64 value)
        {
            append<int64>(value);
            return *this;
        }

        // floating points
        ByteBuffer &operator<<(float value)
        {
            append<float>(value);
            return *this;
        }

        ByteBuffer &operator<<(double value)
        {
            append<double>(value);
            return *this;
        }

        ByteBuffer &operator<<(const std::string &value)
        {
            if (size_t len = value.length())
                append((uint8 const*)value.c_str(), len);
            append((uint8)0);
            return *this;
        }

        ByteBuffer &operator<<(const char *str)
        {
            if (size_t len = (str ? strlen(str) : 0))
                append((uint8 const*)str, len);
            append((uint8)0);
            return *this;
        }

        ByteBuffer &operator>>(bool &value)
        {
            value = read<char>() > 0 ? true : false;
            return *this;
        }

        ByteBuffer &operator>>(uint8 &value)
        {
            value = read<uint8>();
            return *this;
        }

        ByteBuffer &operator>>(uint16 &value)
        {
            value = read<uint16>();
            return *this;
        }

        ByteBuffer &operator>>(uint32 &value)
        {
            value = read<uint32>();
            return *this;
        }

        ByteBuffer &operator>>(uint64 &value)
        {
            value = read<uint64>();
            return *this;
        }

        //signed as in 2e complement
        ByteBuffer &operator>>(int8 &value)
        {
            value = read<int8>();
            return *this;
        }

        ByteBuffer &operator>>(int16 &value)
        {
            value = read<int16>();
            return *this;
        }

        ByteBuffer &operator>>(int32 &value)
        {
            value = read<int32>();
            return *this;
        }

        ByteBuffer &operator>>(int64 &value)
        {
            value = read<int64>();
            return *this;
        }

        ByteBuffer &operator>>(float &value)
        {
            value = read<float>();
            return *this;
        }

        ByteBuffer &operator>>(double &value)
        {
            value = read<double>();
            return *this;
        }

        ByteBuffer &operator>>(std::string& value)
        {
            value.clear();
            while (rpos() < size())                         // prevent crash at wrong string format in packet
            {
                char c = read<char>();
                if (c == 0)
                    break;
                value += c;
            }
            return *this;
        }

        uint8& operator[](size_t const pos)
        {
            if (pos >= size())
                throw ByteBufferPositionException(false, pos, 1, size());
            return _storage[pos];
        }

        uint8 const& operator[](size_t const pos) const
        {
            if (pos >= size())
                throw ByteBufferPositionException(false, pos, 1, size());
            return _storage[pos];
        }

        size_t rpos() const { return _rpos; }

        size_t rpos(size_t rpos_)
        {
            _rpos = rpos_;
            return _rpos;
        }

        void rfinish()
        {
            _rpos = wpos();
        }

        size_t wpos() const { return _wpos; }

        size_t wpos(size_t wpos_)
        {
            _wpos = wpos_;
            return _wpos;
        }

        /// Returns position of last written bit
        size_t bitwpos() const { return _wpos * 8 + 8 - _bitpos; }

        size_t bitwpos(size_t newPos)
        {
            _wpos = newPos / 8;
            _bitpos = 8 - (newPos % 8);
            return _wpos * 8 + 8 - _bitpos;
        }

        template <typename T>
        typename std::enable_if<std::is_arithmetic<T>::value>::type
        read_skip()
        {
            read_skip(sizeof(T));
        }

        template <typename T>
        typename std::enable_if<
            std::is_same<T, char *>::value
                || std::is_same<T, char const *>::value
                || std::is_same<T, std::string>::value
        >::type
        read_skip()
        {
            std::string temp;
            *this >> temp;
        }

        void read_skip(size_t skip)
        {
            if (_rpos + skip > size())
                throw ByteBufferPositionException(false, _rpos, skip, size());
            _rpos += skip;
        }

        template <typename T>
        typename std::enable_if<std::is_arithmetic<T>::value, T>::type
        read()
        {
            T r = read<T>(_rpos);
            _rpos += sizeof(T);
            return r;
        }

        template <typename T>
        typename std::enable_if<std::is_same<T, std::string>::value, T>::type
        read()
        {
            std::string tmp;
            *this >> tmp;
            return tmp;
        }

        template <typename T>
        typename std::enable_if<std::is_arithmetic<T>::value, T>::type
        read(size_t pos) const
        {
            if (pos + sizeof(T) > size())
                throw ByteBufferPositionException(false, pos, sizeof(T), size());
            T val = *((T const*)&_storage[pos]);
            EndianConvert(val);
            return val;
        }

        void read(uint8 *dest, size_t len)
        {
            if (_rpos  + len > size())
               throw ByteBufferPositionException(false, _rpos, len, size());
            memcpy(dest, &_storage[_rpos], len);
            _rpos += len;
        }

        void readPackGUID(uint64& guid)
        {
            if (rpos() + 1 > size())
                throw ByteBufferPositionException(false, _rpos, 1, size());

            guid = 0;

            uint8 guidmark = 0;
            (*this) >> guidmark;

            for (int i = 0; i < 8; ++i)
            {
                if (guidmark & (uint8(1) << i))
                {
                    if (rpos() + 1 > size())
                        throw ByteBufferPositionException(false, _rpos, 1, size());

                    uint8 bit;
                    (*this) >> bit;
                    guid |= (uint64(bit) << (i * 8));
                }
            }
        }

        std::string ReadString(uint32 length)
        {
            if (!length)
                return std::string();
            char* buffer = new char[length + 1];
            memset(buffer, 0, length + 1);
            read((uint8*)buffer, length);
            std::string retval = buffer;
            delete[] buffer;
            return retval;
        }

        //! Method for writing strings that have their length sent separately in packet
        //! without null-terminating the string
        void WriteString(std::string const& str)
        {
            if (size_t len = str.length())
                append(str.c_str(), len);
        }

        uint8 * contents() { return &_storage[0]; }

        const uint8 *contents() const { return &_storage[0]; }

        size_t size() const { return _storage.size(); }
        bool empty() const { return _storage.empty(); }

        void resize(size_t newsize)
        {
            _storage.resize(newsize, 0);
            _rpos = 0;
            _wpos = size();
        }

        void reserve(size_t ressize)
        {
            if (ressize > size())
                _storage.reserve(ressize);
        }

        void append(const char *src, size_t cnt)
        {
            return append((const uint8 *)src, cnt);
        }

        template<class T> void append(const T *src, size_t cnt)
        {
            return append((const uint8 *)src, cnt * sizeof(T));
        }

        void append(const uint8 *src, size_t cnt)
        {
            if (!cnt)
                throw ByteBufferSourceException(_wpos, size(), cnt);

            if (!src)
                throw ByteBufferSourceException(_wpos, size(), cnt);

            ASSERT(size() < 10000000);

            if (_storage.size() < _wpos + cnt)
                _storage.resize(_wpos + cnt);
            memcpy(&_storage[_wpos], src, cnt);
            _wpos += cnt;
        }

        void append(const ByteBuffer& buffer)
        {
            if (buffer.wpos())
                append(buffer.contents(), buffer.wpos());
        }

        // can be used in SMSG_MONSTER_MOVE opcode
        void appendPackXYZ(float x, float y, float z)
        {
            uint32 packed = 0;
            packed |= ((int)(x / 0.25f) & 0x7FF);
            packed |= ((int)(y / 0.25f) & 0x7FF) << 11;
            packed |= ((int)(z / 0.25f) & 0x3FF) << 22;
            *this << packed;
        }

        void appendPackGUID(uint64 guid)
        {
            uint8 packGUID[8+1];
            packGUID[0] = 0;
            size_t size = 1;
            for (uint8 i = 0;guid != 0;++i)
            {
                if (guid & 0xFF)
                {
                    packGUID[0] |= uint8(1 << i);
                    packGUID[size] =  uint8(guid & 0xFF);
                    ++size;
                }

                guid >>= 8;
            }
            append(packGUID, size);
        }

        void put(size_t pos, const uint8 *src, size_t cnt)
        {
            if (pos + cnt > size())
                throw ByteBufferPositionException(true, pos, cnt, size());

            if (!src)
                throw ByteBufferSourceException(_wpos, size(), cnt);

            memcpy(&_storage[pos], src, cnt);
        }

        std::string textlike() const;

        std::string hexlike() const;

        size_t GetBitPos() const
        {
            return _bitpos;
        }

    protected:
        size_t _rpos, _wpos, _bitpos;
        uint8 _curbitval;
        std::vector<uint8> _storage;
};

template <typename T>
inline ByteBuffer &operator<<(ByteBuffer &b, std::vector<T> const &v)
{
    b << (uint32)v.size();
    for (typename std::vector<T>::iterator i = v.begin(); i != v.end(); ++i)
    {
        b << *i;
    }
    return b;
}

template <typename T>
inline ByteBuffer &operator>>(ByteBuffer &b, std::vector<T> &v)
{
    uint32 vsize;
    b >> vsize;
    v.clear();
    while (vsize--)
    {
        T t;
        b >> t;
        v.push_back(t);
    }
    return b;
}

template <typename T>
inline ByteBuffer &operator<<(ByteBuffer &b, std::list<T> const &v)
{
    b << (uint32)v.size();
    for (typename std::list<T>::iterator i = v.begin(); i != v.end(); ++i)
    {
        b << *i;
    }
    return b;
}

template <typename T>
inline ByteBuffer &operator>>(ByteBuffer &b, std::list<T> &v)
{
    uint32 vsize;
    b >> vsize;
    v.clear();
    while (vsize--)
    {
        T t;
        b >> t;
        v.push_back(t);
    }
    return b;
}

template <typename K, typename V>
inline ByteBuffer &operator<<(ByteBuffer &b, std::map<K, V> const &m)
{
    b << (uint32)m.size();
    for (typename std::map<K, V>::iterator i = m.begin(); i != m.end(); ++i)
    {
        b << i->first << i->second;
    }
    return b;
}

template <typename K, typename V>
inline ByteBuffer &operator>>(ByteBuffer &b, std::map<K, V> &m)
{
    uint32 msize;
    b >> msize;
    m.clear();
    while (msize--)
    {
        K k;
        V v;
        b >> k >> v;
        m.insert(make_pair(k, v));
    }
    return b;
}

#endif
