#ifndef _HLSHA1_H
#define _HLSHA1_H

#include "HLCrypt.h"


/* The SHA block size and message digest sizes, in bytes */

#define SHA_DATASIZE    64
#define SHA_DATALEN     16
#define SHA_DIGESTSIZE  20
#define SHA_DIGESTLEN    5

/* The structure for storing SHA info */

struct sha_ctx
	{
	UInt32 digest[SHA_DIGESTLEN];  /* Message digest */
	UInt32 count_l, count_h;       /* 64-bit block count */
	UInt8 block[SHA_DATASIZE];     /* SHA data buffer */
	int index;                        /* index into buffer */
	};

class HLSha1 : public HLHash
{
public:
HLSha1() {}
~HLSha1() {}

virtual UInt32	GetMacLen() { return 20; }
virtual void 	HMAC_XXX(UInt8 *output, DataBuffer key, DataBuffer text);

static UInt8 *GetName() { return "\pHMAC-SHA1"; }

protected:
static void sha_init(struct sha_ctx *ctx);
static void sha_update(struct sha_ctx *ctx, UInt8 *buffer, UInt32 len);
static void sha_final(UInt8 *s, struct sha_ctx *ctx);
static void sha_digest(struct sha_ctx *ctx, UInt8 *s);
static void sha_copy(struct sha_ctx *dest, struct sha_ctx *src);
static int sha_fd(int fd, size_t maxlen, UInt8 *s);
static void sha_block(struct sha_ctx *ctx, UInt8 *block);
static void sha_transform(struct sha_ctx *ctx, UInt32 *data );
};


#endif /* _HLSHA1_H */