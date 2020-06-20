
#include "ServerStorage.h"
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

bool ServerStorage::is_initialized = false;
bool ServerStorage::is_capacity_set = false;

ServerStorage::ServerStorage() {
	
	if (this->is_initialized) {
		throw new runtime_error("ONLY ONE ServerStorage CAN BE USED AT A TIME IN THIS IMPLEMENTATION");
	}
	this->is_initialized = true;
}

void ServerStorage::setCapacity(int totalNumOfBuckets) {
	if (this->is_capacity_set) {
		throw new runtime_error("Capacity of ServerStorage cannot be changed");
	}
	this->is_capacity_set = true;
	this->capacity = totalNumOfBuckets;
	this->buckets.assign(totalNumOfBuckets, Bucket());
}

Bucket ServerStorage::ReadBucket(int position) {
	if (!this->is_capacity_set) {
		throw new runtime_error("Please call setCapacity before reading or writing any block");
	}

	if (position >= this->capacity || position < 0) {
		//std::ostringstream positionStream;
		//positionStream << position;
		throw new runtime_error("You are trying to access Bucket " + to_string(position) + ", but this Server contains only " + to_string(this->capacity) + " buckets.");
	}
	return this->buckets.at(position);
}

void ServerStorage::WriteBucket(int position, const Bucket& bucket_to_write) {
	if (!this->is_capacity_set) {
		throw new runtime_error("Please call setCapacity before reading or writing any block");
	}

	if (position >= this->capacity || position < 0) {
		throw new runtime_error("You are trying to access Bucket " + to_string(position) + ", but this Server contains only " + to_string(this->capacity) + " buckets.");
	}

	this->buckets.at(position) = bucket_to_write;
	return;
}