/******************************************************************************

  Copyright (c) 2014, AudioScience, Inc.
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

   3. Neither the name of the AudioScience, Inc nor the names of its
      contributors may be used to endorse or promote products derived from
      this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.

******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#ifdef __linux__
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#else
typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
#define PRIu64       "I64u"
#define PRIx64       "I64x"
#endif

#include "CppUTest/TestHarness.h"

extern "C"
{

#include "mrp_doubles.h"
#include "mrp.h"
#include "mvrp.h"
#include "parse.h"

    extern struct mvrp_database *MVRP_db;

}

static struct sockaddr_in client;

TEST_GROUP(MvrpTestGroup)
{
    void setup()
    {
        mrpd_reset();
        mvrp_init(1);
    }

    void teardown()
    {
        mvrp_reset();
        mrpd_reset();
    }
};

TEST(MvrpTestGroup, RegisterVLAN)
{
    struct mvrp_attribute a_ref;
    struct mvrp_attribute *a_mvrp = NULL;
    int err_index = 0;
    int parse_status = 0;
	char cmd_string[] = "V++:I=1234";

    CHECK(MVRP_db != NULL);

    /* here we fill in a_ref struct with target values */
	a_ref.attribute = 0x1234;

    /* use string interface to get MSRP to create TalkerAdv attrib in it's database */
    mvrp_recv_cmd(cmd_string, sizeof(cmd_string), &client);

    /* lookup the created attrib */
    a_mvrp = mvrp_lookup(&a_ref);
    CHECK(a_mvrp != NULL);
}

TEST(MvrpTestGroup, TxLVA_clear_tx_flag)
{
	struct mvrp_attribute a_ref;
	struct mvrp_attribute *attrib = NULL;
	int tx_flag_count = 0;
	int err_index = 0;
	int parse_status = 0;
	char cmd_string[] = "V++:I=1234";

	CHECK(MVRP_db != NULL);

	/* here we fill in a_ref struct with target values */
	a_ref.attribute = 0x1234;

	/* use string interface to get MSRP to create TalkerAdv attrib in it's database */
	mvrp_recv_cmd(cmd_string, sizeof(cmd_string), &client);

	/* lookup the created attrib */
	attrib = mvrp_lookup(&a_ref);
	CHECK(attrib != NULL);
	/*
	* Generate a LVA event.
	* This will cause a tx flag to be set for the attribute and then cleared when
	* the attribute is encoded into a PDU.
	*/
	mvrp_event(MRP_EVENT_LVATIMER, NULL);

	/* verify that all tx flags are zero by scanning the attribute list */
	attrib = MVRP_db->attrib_list;
	while (NULL != attrib)
	{
		tx_flag_count += attrib->applicant.tx;
		attrib = attrib->next;
	}
	CHECK(mrpd_send_packet_count() > 0);
	CHECK_EQUAL(0, tx_flag_count);
}
