//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// LibSourcey is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//


#ifdef WIN32
#include <winsock2.h>
#endif


#include "Sourcey/Crypto/HMAC.h"
#include "Sourcey/STUN/Attributes.h"
#include "Sourcey/Logger.h"


using namespace std;


namespace scy {
namespace stun {


Attribute::Attribute(UInt16 type, UInt16 size) : 
	_type(type), _size(size) 
{
}


std::string Attribute::typeString(UInt16 type) 
{
	switch (type) {
	case Attribute::XorMappedAddress: return "XOR-MAPPED-ADDRESS";
	case Attribute::XorPeerAddress: return "XOR-PEER-ADDRESS";
	case Attribute::XorRelayedAddress: return "XOR-RELAYED-ADDRESS";
	case Attribute::MappedAddress: return "MAPPED-ADDRESS";
	case Attribute::ResponseAddress: return "RESPONSE-ADDRESS";
	case Attribute::ChangeRequest: return "CHANGE-REQUEST";		
	case Attribute::SourceAddress: return "SOURCE-ADDRESS";
	case Attribute::ChangedAddress: return "CHANGED-ADDRESS";
	case Attribute::Username: return "USERNAME";
	case Attribute::Password: return "PASSWORD";	
	case Attribute::MessageIntegrity: return "MESSAGE-INTEGRITY";	
	case Attribute::ErrorCode: return "ERROR-CODE";	
	case Attribute::Bandwidth: return "BANDWIDTH";	
	case Attribute::DestinationAddress: return "DESTINATION-ADDRESS";	
	case Attribute::UnknownAttributes: return "UNKNOWN-ATTRIBUTES";	
	case Attribute::ReflectedFrom: return "REFLECTED-FORM";		
	//case Attribute::TransportPreferences: return "TRANSPORT-PREFERENCES";	
	case Attribute::MagicCookie: return "MAGIC-COOKIE";		
	case Attribute::Realm: return "REALM";		
	case Attribute::Nonce: return "NONCE";		
	case Attribute::Software: return "SOFTWARE";	
	case Attribute::Options: return "OPTIONS";		
	case Attribute::AlternateServer: return "ALTERNATE-SERVER";		
	case Attribute::Fingerprint: return "FINGERPRINT";	
	case Attribute::ChannelNumber: return "CHANNEL-NUMBER";		
	case Attribute::Lifetime: return "LIFETIME";	
	case Attribute::Data: return "DATA";
	case Attribute::RequestedTransport: return "REQUESTED-TRANSPORT";	
	case Attribute::ReservationToken: return "RESERVED-TOKEN";	
	case Attribute::EventPort: return "EVEN-PORT";	
	case Attribute::DontFragment: return "DONT-FRAGMENT";	
	case Attribute::ICEControlled: return "ICE-CONTROLLED";	
	case Attribute::ICEControlling: return "ICE-CONTROLLING";	
	case Attribute::ICEPriority: return "PRIORITY";	
	case Attribute::ICEUseCandidate: return "USE-CANDIDATE";				
	case Attribute::ConnectionID: return "CONNECTION-ID";
	default: return "Unknown";
	}
}


UInt16 Attribute::size() const 
{ 
	return _size; 
}


Attribute::Type Attribute::type() const 
{ 
	return static_cast<Attribute::Type>(_type); 
} 


void Attribute::setLength(UInt16 size) 
{ 
	_size = size;
}


string Attribute::typeString() 
{
	return typeString(_type);
}


Attribute* Attribute::create(UInt16 type, UInt16 size)
{
	//Attribute* attr = get(type);
	//if (attr)
	//	return attr;

	switch (type)
	{
	case Attribute::MappedAddress:
		if (size != AddressAttribute::Size)
			return nullptr;
		return new stun::MappedAddress();

	case Attribute::XorMappedAddress:
		if (size != AddressAttribute::Size)
			return nullptr;
		return new stun::XorMappedAddress();

	case Attribute::XorRelayedAddress:
		if (size != AddressAttribute::Size)
			return nullptr;
		return new stun::XorRelayedAddress();

	case Attribute::XorPeerAddress:
		if (size != AddressAttribute::Size)
			return nullptr;
		return new stun::XorPeerAddress();

	case Attribute::AlternateServer:
		if (size != AddressAttribute::Size)
			return nullptr;
		return new stun::AlternateServer();

	case Attribute::ErrorCode:
		if (size < ErrorCode::MinSize)
			return nullptr;
		return new stun::ErrorCode(size);

	case Attribute::UnknownAttributes:
		return new stun::UnknownAttributes(size);

	case Attribute::Fingerprint:
		if (size != UInt32Attribute::Size)
			return nullptr;
		return new stun::Fingerprint();

	case Attribute::RequestedTransport:
		if (size != UInt32Attribute::Size)
			return nullptr;
		return new stun::RequestedTransport();	

	case Attribute::Lifetime:
		if (size != UInt32Attribute::Size)
			return nullptr;
		return new stun::Lifetime();	

	case Attribute::Bandwidth:
		if (size != UInt32Attribute::Size)
			return nullptr;
		return new stun::Bandwidth();	

	case Attribute::ChannelNumber:
		if (size != UInt32Attribute::Size)
			return nullptr;
		return new stun::ICEUseCandidate();
		
	case Attribute::ConnectionID:
		if (size != UInt32Attribute::Size)
			return nullptr;
		return new stun::ConnectionID();

	case Attribute::MessageIntegrity:
		return (size == 20) ? new stun::MessageIntegrity() : nullptr;

	case Attribute::Nonce:
		return (size <= 128) ? new stun::Nonce(size) : nullptr;

	case Attribute::Realm:
		return (size <= 128) ? new stun::Realm(size) : nullptr;

	case Attribute::Software:
		return (size <= 128) ? new stun::Software(size) : nullptr;

	case Attribute::ReservationToken:
		return (size == 8) ? new stun::ReservationToken() : nullptr;		

	case Attribute::MagicCookie:
		return (size == 4) ? new stun::MagicCookie() : nullptr;		

	case Attribute::Data:
		return new stun::Data(size);

	case Attribute::Username:
		return (size <= 128) ? new stun::Username(size) : nullptr;

	case Attribute::Password:
		return (size <= 128) ? new stun::Password(size) : nullptr;

	case Attribute::ICEPriority:
		if (size != UInt32Attribute::Size)
			return nullptr;
		return new stun::ICEPriority();

	case Attribute::ICEControlled:
		if (size != UInt64Attribute::Size)
			return nullptr;
		return new stun::ICEControlled();

	case Attribute::ICEControlling:
		if (size != UInt64Attribute::Size)
			return nullptr;
		return new stun::ICEControlling();

	case Attribute::ICEUseCandidate:
		return (size == 0) ? new stun::ICEUseCandidate() : nullptr;
		
	case Attribute::DontFragment:
		if (size != FlagAttribute::Size)
			return nullptr;
		return new stun::DontFragment();
		
	case Attribute::EventPort:
		if (size != UInt8Attribute::Size)
			return nullptr;
		return new stun::EventPort();

	//case Attribute::UnknownAttributes:
	//	return (size % 2 == 0) ? new stun::UnknownAttributes(size) : nullptr;
	//	break;

	//case Attribute::TransportPrefs:
	//	if ((size != TransportPrefs::Size1) &&
	//		(size != TransportPrefs::Size2))
	//		return nullptr;
	//	return new stun::TransportPrefs(size);

	//case Attribute::MagicCookie:
	//	return (size == 4) ? new stun::MagicCookie() : nullptr;
	//	break;

	default:
		errorL() << "[STUN] Unable to create attribute for type: " << type << endl;
		break;
	}

	//_attrs.push_back(attr);
	//return attr;
	//assert(false);
	return nullptr;
}


// ---------------------------------------------------------------------------
//
AddressAttribute::AddressAttribute(UInt16 type) : 
	Attribute(type, Size), _family(0), _port(0), _ip(0) 
{
}


AddressAttribute::AddressAttribute(const AddressAttribute& r) :
	Attribute(r._type, Size),
	_family(r._family),
	_port(r._port),
	_ip(r._ip)
{
}


Attribute* AddressAttribute::clone() 
{
	return new AddressAttribute(*this);
}


string AddressAttribute::ipString() const 
{ 
	ostringstream ost;
	ost << ((_ip >> 24) & 0xff);
	ost << '.';
	ost << ((_ip >> 16) & 0xff);
	ost << '.';
	ost << ((_ip >> 8) & 0xff);
	ost << '.';
	ost << ((_ip >> 0) & 0xff);
	return ost.str();
}


net::Address AddressAttribute::address() const 
{ 
	return net::Address(ipString(), _port); 
}


void AddressAttribute::setIP(const std::string& ip) 
{ 
	in_addr address;
	address.s_addr = inet_addr(ip.c_str());
	_ip = ntohl(address.s_addr);
}


void AddressAttribute::read(BitReader& reader) 
{
	UInt8 dummy;
	reader.getU8(dummy);
	reader.getU8(_family);
	reader.getU16(_port);
	reader.getU32(_ip);
}


void AddressAttribute::write(BitWriter& writer) const 
{
	writer.putU8(0);
	writer.putU8(_family);
	writer.putU16(_port);
	writer.putU32(_ip);
}


// ---------------------------------------------------------------------------
//
UInt8Attribute::UInt8Attribute(UInt16 type) : 
	Attribute(type, Size), _bits(0) 
{
}


UInt8Attribute::UInt8Attribute(const UInt8Attribute& r) :
	Attribute(r._type, Size),
	_bits(r._bits)
{
}


Attribute* UInt8Attribute::clone() 
{
	return new UInt8Attribute(*this);
}


bool UInt8Attribute::getBit(int index) const 
{
	assert((0 <= index) && (index < 32));
	return static_cast<bool>((_bits >> index) & 0x1);
}


void UInt8Attribute::setBit(int index, bool value) 
{
	assert((0 <= index) && (index < 32));
	_bits &=  ~(1 << index);
	_bits |=  value ? (1 << index) : 0;
}


void UInt8Attribute::read(BitReader& reader) 
{
	reader.getU8(_bits);
}


void UInt8Attribute::write(BitWriter& writer) const 
{
	writer.putU8(_bits);
}


// ---------------------------------------------------------------------------
//
UInt32Attribute::UInt32Attribute(UInt16 type) : 
	Attribute(type, Size), _bits(0) 
{
}	


UInt32Attribute::UInt32Attribute(const UInt32Attribute& r) :
	Attribute(r._type, Size),
	_bits(r._bits)
{
}


Attribute* UInt32Attribute::clone() 
{
	return new UInt32Attribute(*this);
}


bool UInt32Attribute::getBit(int index) const 
{
	assert((0 <= index) && (index < 32));
	return static_cast<bool>((_bits >> index) & 0x1);
}


void UInt32Attribute::setBit(int index, bool value) 
{
	assert((0 <= index) && (index < 32));
	_bits &=  ~(1 << index);
	_bits |=  value ? (1 << index) : 0;
}


void UInt32Attribute::read(BitReader& reader) 
{
	reader.getU32(_bits);
}

void UInt32Attribute::write(BitWriter& writer) const 
{
	writer.putU32(_bits);
}


// ---------------------------------------------------------------------------
//
UInt64Attribute::UInt64Attribute(UInt16 type) : 
	Attribute(type, Size), _bits(0) 
{
}


UInt64Attribute::UInt64Attribute(const UInt64Attribute& r) :
	Attribute(r._type, Size),
	_bits(r._bits)
{
}


Attribute* UInt64Attribute::clone() 
{
	return new UInt64Attribute(*this);
}


bool UInt64Attribute::getBit(int index) const 
{
	assert((0 <= index) && (index < 32));
	return static_cast<bool>((_bits >> index) & 0x1);
}


void UInt64Attribute::setBit(int index, bool value) 
{
	assert((0 <= index) && (index < 32));
	_bits &=  ~(1 << index);
	_bits |=  value ? (1 << index) : 0;
}


void UInt64Attribute::read(BitReader& reader) 
{
	reader.getU64(_bits);
}


void UInt64Attribute::write(BitWriter& writer) const 
{
	writer.putU64(_bits);
}


// ---------------------------------------------------------------------------
//
FlagAttribute::FlagAttribute(UInt16 type) : 
	Attribute(type, 0) 
{
}


Attribute* FlagAttribute::clone() 
{
	return new FlagAttribute(type());
}


// ---------------------------------------------------------------------------
//
StringAttribute::StringAttribute(UInt16 type, UInt16 size) : 
	Attribute(type, size), _bytes(0) 
{
}


StringAttribute::StringAttribute(const StringAttribute& r) :
	Attribute(r._type, r._size), _bytes(0) 
{
	copyBytes(r._bytes, r._size);
}


StringAttribute::~StringAttribute() 
{
	if (_bytes)
		delete [] _bytes;
}


Attribute* StringAttribute::clone() 
{
	return new StringAttribute(*this);
}


void StringAttribute::setBytes(char* bytes, unsigned size) 
{
	if (_bytes)
		delete [] _bytes;
	_bytes = bytes;
	setLength(size);
}


void StringAttribute::copyBytes(const char* bytes) 
{
	copyBytes(bytes, static_cast<UInt16>(strlen(bytes)));
}


void StringAttribute::copyBytes(const void* bytes, unsigned size) 
{
	char* newBytes = new char[size];
	memcpy(newBytes, bytes, size);
	setBytes(newBytes, size);
}


UInt8 StringAttribute::getByte(int index) const 
{
	assert(_bytes != nullptr);
	assert((0 <= index) && (index < size()));
	return static_cast<UInt8>(_bytes[index]);
}


void StringAttribute::setByte(int index, UInt8 value) 
{
	assert(_bytes != nullptr);
	assert((0 <= index) && (index < size()));
	_bytes[index] = value;
}


void StringAttribute::read(BitReader& reader) 
{
	if (_bytes)
		delete [] _bytes;
	_bytes = new char[size()];	
	reader.get(_bytes, size());
}


void StringAttribute::write(BitWriter& writer) const 
{
	if (_bytes)
		writer.put(_bytes, size());
}


string StringAttribute::asString() const 
{
	return std::string(_bytes, size());
}


/*
//--------------- Fingerprint ----------------
Fingerprint::Fingerprint() :
	UInt32Attribute(Attribute::Fingerprint) {
}


//void Fingerprint::setCRC32(unsigned int crc) {
//	_crc32 = crc;
//}


//unsigned int Fingerprint::crc32() {
//	return _crc32;
//}
*/


	/*
void Fingerprint::write(BitWriter& writer) const {
	writer.putU32(_crc32);
}


bool Fingerprint::read(BitReader& reader) {
	try {
		reader.getU32(_crc32);
	}
	catch(...) {
		return false;
	}
	return true;
}
*/


// ---------------------------------------------------------------------------
//
MessageIntegrity::MessageIntegrity() : 
	Attribute(Attribute::MessageIntegrity, Size) 
{
}
	

MessageIntegrity::MessageIntegrity(const MessageIntegrity& r) :
	Attribute(r._type, Size),
	_input(r._input),
	_hmac(r._hmac),
	_key(r._key)
{
}


MessageIntegrity::~MessageIntegrity() 
{
}



Attribute* MessageIntegrity::clone() 
{
	return new MessageIntegrity(*this);
}

	
bool MessageIntegrity::verifyHmac(const std::string& key) const 
{
	//debugL() << "Message: Verify HMAC: " << key << endl;

	assert(key.size());
	assert(!_hmac.empty());
	assert(!_input.empty());

	//debugL() << "Message: Packet integrity input (" << _input << ")" << endl;
	//debugL() << "Message: Packet integrity key (" << key << ")" << endl;

	std::string hmac = crypto::computeHMAC(_input, key);
	assert(hmac.size() == Size);

	//debugL() << "Message: Verifying message integrity (" << hmac << ": " << _hmac << ")" << endl;

	return _hmac == hmac;
}


void MessageIntegrity::read(BitReader& reader) 
{
	//debugL() << "Message: Read HMAC" << endl;

	// Read the HMAC value.
	reader.get(_hmac, MessageIntegrity::Size);

	//debugL() << "Message: Parsed message integrity (" << _hmac << ")" << endl;

	// Remember the original position and set the buffer position back to 0.	
	int originalPos = reader.position();
	reader.seek(0);

	// Get the message prior to the current attribute and fill the
	// attribute with dummy content.
	Buffer hmacBuf;
	BitWriter hmacWriter(hmacBuf);
	//Buffer hmacBuf(buf.data(), buf.available() - MessageIntegrity::Size);
	hmacWriter.put(reader.begin(), originalPos); //reader.available() - MessageIntegrity::Size
	hmacWriter.put("00000000000000000000");

	// Ensure the STUN message size value represents the real 
	// size of the buffered message.
	//hmacBuf.updateU32((UInt32)reader.available(), 2);
	hmacWriter.updateU32((UInt32)hmacWriter.position(), 2);
	_input.assign(hmacWriter.begin(), hmacWriter.position());
	  
	// Reset the original buffer position.
	reader.seek(originalPos);
}


void MessageIntegrity::write(BitWriter& writer) const 
{
	//debugL() << "Message: Write HMAC" << endl;

	// If the key (password) is present we will compute the 
	// HMAC for the current message, otherwise the attribute
	// content will be copied.
	if (!_key.empty()) {

		// Get the message prior to the current attribute and
		// fill the attribute with dummy content.
		//Buffer hmacBuf(buf.c_str(), reader.available());		
		Buffer hmacBuf(1024); // TODO: alloc exact size
		BitWriter hmacWriter(hmacBuf);
		//hmacBuf.put(buf.data(), buf.available());
		hmacWriter.put(writer.begin(), writer.position());
		hmacWriter.put("00000000000000000000");

		// Ensure the STUN message size value represents the real 
		// size of the buffered message.
		hmacWriter.updateU32((UInt32)hmacWriter.position() + MessageIntegrity::Size, 2);
		
		//string input(hmacBuf.data(), hmacBuf.available());
		std::string input(hmacWriter.begin(), hmacWriter.position());
		std::string hmac(crypto::computeHMAC(input, _key));
		assert(hmac.size() == MessageIntegrity::Size);

		// Append the real HAMC to the buffer.
		writer.put(hmac.c_str(), MessageIntegrity::Size);
	}
	else {
		assert(!_hmac.empty());
		writer.put(_hmac.c_str(), MessageIntegrity::Size);
	}
}


// ---------------------------------------------------------------------------
//
ErrorCode::ErrorCode(UInt16 size) : 
	Attribute(Attribute::ErrorCode, size), _class(0), _number(0) 
{
	assert(size >= MinSize);
}
	

ErrorCode::ErrorCode(const ErrorCode& r) :
	Attribute(Attribute::ErrorCode, r._size),
	_class(r._class),
	_number(r._number),
	_reason(r._reason)
{
}


ErrorCode::~ErrorCode() 
{
}


Attribute* ErrorCode::clone() 
{
	return new ErrorCode(*this);
}
	

UInt32 ErrorCode::errorCode() const 
{ 
	return (_class << 8) | _number;
}


void ErrorCode::setErrorCode(UInt32 code) 
{
	if ((code >> 11) != 0)
		throw std::runtime_error("error-code bits not zero");

	_class = (UInt8)((code >> 8) & 0x7);
	_number = (UInt8)(code & 0xff);
}


void ErrorCode::setReason(const std::string& reason) 
{
	setLength(MinSize + static_cast<UInt16>(reason.size()));
	_reason = reason;
}

void ErrorCode::read(BitReader& reader) 
{
	UInt32 val;
	reader.getU32(val);

	setErrorCode(val);

	reader.get(_reason, size() - 4);
}

void ErrorCode::write(BitWriter& writer) const 
{
	writer.putU32(errorCode());
	writer.put(_reason);
}


// ---------------------------------------------------------------------------
//
UInt16ListAttribute::UInt16ListAttribute(UInt16 type, UInt16 size) : 
	Attribute(type, size) 
{
}


UInt16ListAttribute::UInt16ListAttribute(const UInt16ListAttribute& r) :
	Attribute(r._type, r._size),
	_attrTypes(r._attrTypes)
{
	/*
	//UInt16ListAttribute* attr = new UInt16ListAttribute(type(), size());
	for (unsigned i = 0; i < r._attrTypes.size(); i++)
		addType((r._attrTypes)[i]);
	return attr;
	*/
}


UInt16ListAttribute::~UInt16ListAttribute() 
{
}


Attribute* UInt16ListAttribute::clone() 
{
	return new UInt16ListAttribute(*this);
}


size_t UInt16ListAttribute::size() const 
{
	return _attrTypes.size();
}


UInt16 UInt16ListAttribute::getType(int index) const 
{
	return _attrTypes[index];
}


void UInt16ListAttribute::setType(int index, UInt16 value) 
{
	_attrTypes[index] = value;
}


void UInt16ListAttribute::addType(UInt16 value) 
{
	_attrTypes.push_back(value);
	setLength(static_cast<UInt16>(_attrTypes.size() * 2));
}


void UInt16ListAttribute::read(BitReader& reader) 
{
	for (unsigned i = 0; i < size() / 2; i++) {
		UInt16 attr;
		reader.getU16(attr);
		_attrTypes.push_back(attr);
	}
}

void UInt16ListAttribute::write(BitWriter& writer) const 
{
	for (unsigned i = 0; i < _attrTypes.size(); i++)
		writer.putU16(_attrTypes[i]);
}


} } // namespace scy:stun
