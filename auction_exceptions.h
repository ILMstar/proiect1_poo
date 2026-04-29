#pragma once

#include <stdexcept>
#include <string>

class AuctionError : public std::runtime_error {
public:
    explicit AuctionError(const std::string& msg) : std::runtime_error(msg) {}
};

class FileError : public AuctionError {
public:
    explicit FileError(const std::string& msg) : AuctionError(msg) {}
};

class PlayerError : public AuctionError {
public:
    explicit PlayerError(const std::string& msg) : AuctionError(msg) {}
};

class ItemError : public AuctionError {
public:
    explicit ItemError(const std::string& msg) : AuctionError(msg) {}
};
