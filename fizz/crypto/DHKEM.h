// Copyright 2004-present Facebook. All Rights Reserved.

#pragma once

#include <fizz/crypto/exchange/OpenSSLKeyExchange.h>
#include <fizz/crypto/ECCurve.h>
#include <fizz/crypto/HpkeHkdf.h>
#include <fizz/record/Types.h>

namespace fizz {

/**
 * This class implements the DHKEM functions as specified in this IETF RFC.
 * https://datatracker.ietf.org/doc/draft-irtf-cfrg-hpke/?include_text=1
 *
 */
class DHKEM {
  public:

  struct EncapResult {
    std::unique_ptr<folly::IOBuf> sharedSecret;
    std::unique_ptr<folly::IOBuf> enc;
  };

  DHKEM(std::unique_ptr<KeyExchange> kex, NamedGroup group, std::unique_ptr<fizz::hpke::Hkdf> hkdf);

  /**
  * Generate an ephemeral, fixed-length symmetric key
  * (the KEM shared secret) and a fixed-length encapsulation of
  * that key that can be decapsulated by the holder of the private
  * key corresponding to "pk"
  */
  EncapResult encap(folly::ByteRange pkR);
  /**
  * Use the private key "sk" to recover the
  * ephemeral symmetric key (the KEM shared secret) from its
  * encapsulated representation "enc"
  */
  std::unique_ptr<folly::IOBuf> decap(folly::ByteRange enc);

 private:
  std::unique_ptr<folly::IOBuf> extractAndExpand(std::unique_ptr<folly::IOBuf> dh, std::unique_ptr<folly::IOBuf> kemContext);
  std::unique_ptr<KeyExchange> kex_;
  NamedGroup group_;
  std::unique_ptr<fizz::hpke::Hkdf> hkdf_;

};
} // namespace fizz
