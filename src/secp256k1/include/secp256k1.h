#ifndef _VLCP256K1_
# define _VLCP256K1_

# ifdef __cplusplus
extern "C" {
# endif

# if !defined(VLCP256K1_GNUC_PREREQ)
#  if defined(__GNUC__)&&defined(__GNUC_MINOR__)
#   define VLCP256K1_GNUC_PREREQ(_maj,_min) \
 ((__GNUC__<<16)+__GNUC_MINOR__>=((_maj)<<16)+(_min))
#  else
#   define VLCP256K1_GNUC_PREREQ(_maj,_min) 0
#  endif
# endif

# if (!defined(__STDC_VERSION__) || (__STDC_VERSION__ < 199901L) )
#  if VLCP256K1_GNUC_PREREQ(2,7)
#   define VLCP256K1_INLINE __inline__
#  elif (defined(_MSC_VER))
#   define VLCP256K1_INLINE __inline
#  else
#   define VLCP256K1_INLINE
#  endif
# else
#  define VLCP256K1_INLINE inline
# endif

/**Warning attributes
  * NONNULL is not used if VLCP256K1_BUILD is set to avoid the compiler optimizing out
  * some paranoid null checks. */
# if defined(__GNUC__) && VLCP256K1_GNUC_PREREQ(3, 4)
#  define VLCP256K1_WARN_UNUSED_RESULT __attribute__ ((__warn_unused_result__))
# else
#  define VLCP256K1_WARN_UNUSED_RESULT
# endif
# if !defined(VLCP256K1_BUILD) && defined(__GNUC__) && VLCP256K1_GNUC_PREREQ(3, 4)
#  define VLCP256K1_ARG_NONNULL(_x)  __attribute__ ((__nonnull__(_x)))
# else
#  define VLCP256K1_ARG_NONNULL(_x)
# endif


/** Flags to pass to secp256k1_start. */
# define VLCP256K1_START_VERIFY (1 << 0)
# define VLCP256K1_START_SIGN   (1 << 1)

/** Initialize the library. This may take some time (10-100 ms).
 *  You need to call this before calling any other function.
 *  It cannot run in parallel with any other functions, but once
 *  secp256k1_start() returns, all other functions are thread-safe.
 */
void secp256k1_start(unsigned int flags);

/** Free all memory associated with this library. After this, no
 *  functions can be called anymore, except secp256k1_start()
 */
void secp256k1_stop(void);

/** Verify an ECDSA signature.
 *  Returns: 1: correct signature
 *           0: incorrect signature
 *          -1: invalid public key
 *          -2: invalid signature
 * In:       msg:       the message being verified (cannot be NULL)
 *           msglen:    the length of the message (at most 32)
 *           sig:       the signature being verified (cannot be NULL)
 *           siglen:    the length of the signature
 *           pubkey:    the public key to verify with (cannot be NULL)
 *           pubkeylen: the length of pubkey
 * Requires starting using VLCP256K1_START_VERIFY.
 */
VLCP256K1_WARN_UNUSED_RESULT int secp256k1_ecdsa_verify(
  const unsigned char *msg,
  int msglen,
  const unsigned char *sig,
  int siglen,
  const unsigned char *pubkey,
  int pubkeylen
) VLCP256K1_ARG_NONNULL(1) VLCP256K1_ARG_NONNULL(3) VLCP256K1_ARG_NONNULL(5);

/** Create an ECDSA signature.
 *  Returns: 1: signature created
 *           0: nonce invalid, try another one
 *  In:      msg:    the message being signed (cannot be NULL)
 *           msglen: the length of the message being signed (at most 32)
 *           vlckey: pointer to a 32-byte vlcret key (cannot be NULL, assumed to be valid)
 *           nonce:  pointer to a 32-byte nonce (cannot be NULL, generated with a cryptographic PRNG)
 *  Out:     sig:    pointer to an array where the signature will be placed (cannot be NULL)
 *  In/Out:  siglen: pointer to an int with the length of sig, which will be updated
 *                   to contain the actual signature length (<=72).
 * Requires starting using VLCP256K1_START_SIGN.
 */
VLCP256K1_WARN_UNUSED_RESULT int secp256k1_ecdsa_sign(
  const unsigned char *msg,
  int msglen,
  unsigned char *sig,
  int *siglen,
  const unsigned char *vlckey,
  const unsigned char *nonce
) VLCP256K1_ARG_NONNULL(1) VLCP256K1_ARG_NONNULL(3) VLCP256K1_ARG_NONNULL(4) VLCP256K1_ARG_NONNULL(5) VLCP256K1_ARG_NONNULL(6);

/** Create a compact ECDSA signature (64 byte + recovery id).
 *  Returns: 1: signature created
 *           0: nonce invalid, try another one
 *  In:      msg:    the message being signed (cannot be NULL)
 *           msglen: the length of the message being signed (at most 32)
 *           vlckey: pointer to a 32-byte vlcret key (cannot be NULL, assumed to be valid)
 *           nonce:  pointer to a 32-byte nonce (cannot be NULL, generated with a cryptographic PRNG)
 *  Out:     sig:    pointer to a 64-byte array where the signature will be placed (cannot be NULL)
 *           recid:  pointer to an int, which will be updated to contain the recovery id (can be NULL)
 * Requires starting using VLCP256K1_START_SIGN.
 */
VLCP256K1_WARN_UNUSED_RESULT int secp256k1_ecdsa_sign_compact(
  const unsigned char *msg,
  int msglen,
  unsigned char *sig64,
  const unsigned char *vlckey,
  const unsigned char *nonce,
  int *recid
) VLCP256K1_ARG_NONNULL(1) VLCP256K1_ARG_NONNULL(3) VLCP256K1_ARG_NONNULL(4) VLCP256K1_ARG_NONNULL(5);

/** Recover an ECDSA public key from a compact signature.
 *  Returns: 1: public key successfully recovered (which guarantees a correct signature).
 *           0: otherwise.
 *  In:      msg:        the message assumed to be signed (cannot be NULL)
 *           msglen:     the length of the message (at most 32)
 *           sig64:      signature as 64 byte array (cannot be NULL)
 *           compressed: whether to recover a compressed or uncompressed pubkey
 *           recid:      the recovery id (0-3, as returned by ecdsa_sign_compact)
 *  Out:     pubkey:     pointer to a 33 or 65 byte array to put the pubkey (cannot be NULL)
 *           pubkeylen:  pointer to an int that will contain the pubkey length (cannot be NULL)
 * Requires starting using VLCP256K1_START_VERIFY.
 */
VLCP256K1_WARN_UNUSED_RESULT int secp256k1_ecdsa_recover_compact(
  const unsigned char *msg,
  int msglen,
  const unsigned char *sig64,
  unsigned char *pubkey,
  int *pubkeylen,
  int compressed,
  int recid
) VLCP256K1_ARG_NONNULL(1) VLCP256K1_ARG_NONNULL(3) VLCP256K1_ARG_NONNULL(4) VLCP256K1_ARG_NONNULL(5);

/** Verify an ECDSA vlcret key.
 *  Returns: 1: vlcret key is valid
 *           0: vlcret key is invalid
 *  In:      vlckey: pointer to a 32-byte vlcret key (cannot be NULL)
 */
VLCP256K1_WARN_UNUSED_RESULT int secp256k1_ec_vlckey_verify(const unsigned char *vlckey) VLCP256K1_ARG_NONNULL(1);

/** Just validate a public key.
 *  Returns: 1: valid public key
 *           0: invalid public key
 *  In:      pubkey:    pointer to a 33-byte or 65-byte public key (cannot be NULL).
 *           pubkeylen: length of pubkey
 */
VLCP256K1_WARN_UNUSED_RESULT int secp256k1_ec_pubkey_verify(const unsigned char *pubkey, int pubkeylen) VLCP256K1_ARG_NONNULL(1);

/** Compute the public key for a vlcret key.
 *  In:     compressed: whether the computed public key should be compressed
 *          vlckey:     pointer to a 32-byte private key (cannot be NULL)
 *  Out:    pubkey:     pointer to a 33-byte (if compressed) or 65-byte (if uncompressed)
 *                      area to store the public key (cannot be NULL)
 *          pubkeylen:  pointer to int that will be updated to contains the pubkey's
 *                      length (cannot be NULL)
 *  Returns: 1: vlcret was valid, public key stores
 *           0: vlcret was invalid, try again.
 * Requires starting using VLCP256K1_START_SIGN.
 */
VLCP256K1_WARN_UNUSED_RESULT int secp256k1_ec_pubkey_create(
  unsigned char *pubkey,
  int *pubkeylen,
  const unsigned char *vlckey,
  int compressed
) VLCP256K1_ARG_NONNULL(1) VLCP256K1_ARG_NONNULL(2) VLCP256K1_ARG_NONNULL(3);

/** Decompress a public key.
 * In/Out: pubkey:    pointer to a 65-byte array to put the decompressed public key.
                      It must contain a 33-byte or 65-byte public key already (cannot be NULL)
 *         pubkeylen: pointer to the size of the public key pointed to by pubkey (cannot be NULL)
                      It will be updated to reflect the new size.
 * Returns: 0 if the passed public key was invalid, 1 otherwise. If 1 is returned, the
            pubkey is replaced with its decompressed version.
 */
VLCP256K1_WARN_UNUSED_RESULT int secp256k1_ec_pubkey_decompress(
  unsigned char *pubkey,
  int *pubkeylen
) VLCP256K1_ARG_NONNULL(1) VLCP256K1_ARG_NONNULL(2);

/** Export a private key in DER format. */
VLCP256K1_WARN_UNUSED_RESULT int secp256k1_ec_privkey_export(
  const unsigned char *vlckey,
  unsigned char *privkey,
  int *privkeylen,
  int compressed
) VLCP256K1_ARG_NONNULL(1) VLCP256K1_ARG_NONNULL(2) VLCP256K1_ARG_NONNULL(3);

/** Import a private key in DER format. */
VLCP256K1_WARN_UNUSED_RESULT int secp256k1_ec_privkey_import(
  unsigned char *vlckey,
  const unsigned char *privkey,
  int privkeylen
) VLCP256K1_ARG_NONNULL(1) VLCP256K1_ARG_NONNULL(2);

/** Tweak a private key by adding tweak to it. */
VLCP256K1_WARN_UNUSED_RESULT int secp256k1_ec_privkey_tweak_add(
  unsigned char *vlckey,
  const unsigned char *tweak
) VLCP256K1_ARG_NONNULL(1) VLCP256K1_ARG_NONNULL(2);

/** Tweak a public key by adding tweak times the generator to it.
 * Requires starting with VLCP256K1_START_VERIFY.
 */
VLCP256K1_WARN_UNUSED_RESULT int secp256k1_ec_pubkey_tweak_add(
  unsigned char *pubkey,
  int pubkeylen,
  const unsigned char *tweak
) VLCP256K1_ARG_NONNULL(1) VLCP256K1_ARG_NONNULL(3);

/** Tweak a private key by multiplying it with tweak. */
VLCP256K1_WARN_UNUSED_RESULT int secp256k1_ec_privkey_tweak_mul(
  unsigned char *vlckey,
  const unsigned char *tweak
) VLCP256K1_ARG_NONNULL(1) VLCP256K1_ARG_NONNULL(2);

/** Tweak a public key by multiplying it with tweak.
 * Requires starting with VLCP256K1_START_VERIFY.
 */
VLCP256K1_WARN_UNUSED_RESULT int secp256k1_ec_pubkey_tweak_mul(
  unsigned char *pubkey,
  int pubkeylen,
  const unsigned char *tweak
) VLCP256K1_ARG_NONNULL(1) VLCP256K1_ARG_NONNULL(3);

# ifdef __cplusplus
}
# endif

#endif
