// Class automatically generated by Dev-C++ New Class wizard

#include <protocol/Packet.h> // class's header file
#include <string.h>

namespace protocol {

// class constructor
Packet::Packet()
{
	this->actualLength = MIN_PACKET_SIZE;
	this->resetDataIdx();
	// insert your code here
}

// class destructor
Packet::~Packet()
{
	// insert your code here
}

char * Packet::getPacket(){
	return packet;
}

char Packet::getActualLength(){
	return actualLength;
}

char Packet::getOriginGroup(){
	return this->getGroup(ORIGIN_FIELD);
}

char Packet::getOriginId(){
	return this->getId(ORIGIN_FIELD);
}

char Packet::getDestinationGroup(){
	return this->getGroup(DESTINY_FIELD);
}

char Packet::getDestinationId(){
	return this->getId(DESTINY_FIELD);
}

void Packet::setOriginGroup(char group){
	return this->setGroup(ORIGIN_FIELD, group);
}

void Packet::setDestinationGroup(char group){
	return this->setGroup(DESTINY_FIELD, group);
}

void Packet::setOriginId(char id){
	return this->setId(ORIGIN_FIELD,id);
}

void Packet::setDestinationId(char id){
	return this->setId(DESTINY_FIELD, id);
}

char Packet::getGroup(int field){
	char aux = packet[field];
	return aux >> 4;
}

char Packet::getId(int field){
	char aux = packet[field];
	return aux & 0x0F;
}

void Packet::setGroup(int field, char group){
	// Clear Group
	packet[field] &= 0x0F;

	// Set Group
	char aux = group << 4;
	packet[field] |= aux;
}

void Packet::setId(int field, char id){
	// Clear Id
	packet[field] &= 0xF0;

	// Set Id
	char aux = id & 0x0F;
	packet[field] |= aux;
}

bool Packet::checkCRC(){
	int length = this->actualLength - 1;
	char calculated = 0;
	int i=0;
	for ( i = 0 ; i < length ; i++ )
	    calculated ^= this->packet[i];
	return this->packet[i] == calculated;
}

void Packet::calculateCRC(){
	int length = this->actualLength - 1;
	char calculated = 0;
	int i=0;
	for ( i = 0 ; i < length ; i++ )
	    calculated ^= this->packet[i];
	this->packet[i] = calculated;
}

void Packet::setBroadcast(){
	this->setDestinationGroup(0x0F);
	this->setGroupBroadcast();
}

void Packet::setGroupBroadcast(){
   	this->setDestinationId(0x0F);
}

bool Packet::isResponse(){
	return (this->packet[COMMAND_FIELD] & 0x80 ) == 0x80;
}

void Packet::setResponse(){
	this->packet[COMMAND_FIELD] = this->packet[COMMAND_FIELD] | 0x80;
}

void Packet::setCommand(char command){
	this->packet[COMMAND_FIELD] = command & 0x7F;
}

char Packet::getCommand(){
	return this->packet[COMMAND_FIELD] & 0x7F;
}

bool Packet::isACK(){
	return this->getDataLength() == 0;
}

char * Packet::getData(){
	return &(this->packet[DATA_FIELD]);
}

char Packet::getDataLength(){
	return this->actualLength - MIN_PACKET_SIZE;
}

void Packet::addData(char * data, char length){
	if ( length < 1 )
	    return;

	char i;
	for( i = 0 ; i < length ; i++ )
	    this->addData(data[i]);

}

void Packet::addData(short data){
	this->addData((char)(data & 0xFF00));
	this->addData((char)(data & 0x00FF));
}

void Packet::addData(char data){
    packet[this->dataIdx] = data;
    this->dataIdx++;
	this->actualLength += 1;
}

void Packet::resetDataIdx(){
	this->dataIdx = DATA_FIELD;
}

void Packet::clear(){
	memset(this->packet,'\0',this->actualLength);
}

char Packet::getCharData(){
    return packet[this->dataIdx++];
}

short Packet::getShortData(){
    short up = packet[this->dataIdx++] & 0x00FF;
    short down = packet[this->dataIdx++] & 0x00FF;
    short ret = 0xFFFF;
    ret &= ( down << 8 );
    ret &= up;
    return ret;
}

int Packet::getIntData(){
    int up = packet[this->dataIdx++] & 0x00FF;
    int down = packet[this->dataIdx++] & 0x00FF;
    int ret = 0xFFFF;
    ret &= ( up << 8 );
    ret &= down;
    return ret;
}

}
