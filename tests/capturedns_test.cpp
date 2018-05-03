/*
 * Copyright 2016-2018 Internet Corporation for Assigned Names and Numbers.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, you can obtain one at https://mozilla.org/MPL/2.0/.
 */

/*
 * Developed by Sinodun IT (www.sinodun.com)
 */

#include <vector>

#include "catch.hpp"

#define private public
#include "capturedns.hpp"
#undef private

#include "config.h"

SCENARIO("Serialising DNS packets", "[dnspacket]")
{
    GIVEN("A sample DNS message with one compression")
    {
        CaptureDNS msg;
        msg.id(0x6692);
        msg.type(CaptureDNS::RESPONSE);
        msg.add_query(
            CaptureDNS::query(
                CaptureDNS::encode_domain_name("sec2.apnic.com"),
                CaptureDNS::A,
                CaptureDNS::IN
                )
            );
        msg.add_authority(
            CaptureDNS::resource(
                CaptureDNS::encode_domain_name("com"),
                CaptureDNS::encode_domain_name("a.gtld-servers.net"),
                CaptureDNS::NS,
                CaptureDNS::IN,
                172800)
            );

        THEN("Message is serialised with compressed name")
        {
            std::vector<uint8_t> EXPECTED
                { 0x66,0x92,0x80,0x00,0x00,0x01,0x00,0x00,
                  0x00,0x01,0x00,0x00,0x04,0x73,0x65,0x63,
                  0x32,0x05,0x61,0x70,0x6e,0x69,0x63,0x03,
                  0x63,0x6f,0x6d,0x00,0x00,0x01,0x00,0x01,
                  0xc0,0x17,0x00,0x02,0x00,0x01,0x00,0x02,
                  0xa3,0x00,0x00,0x14,0x01,0x61,0x0c,0x67,
                  0x74,0x6c,0x64,0x2d,0x73,0x65,0x72,0x76,
                  0x65,0x72,0x73,0x03,0x6e,0x65,0x74,0x00
                };
            uint32_t pkt_size = msg.header_size();
            REQUIRE(pkt_size == EXPECTED.size());

            std::vector<uint8_t> buf(pkt_size);
#ifdef HAVE_LIBTINS4
            msg.write_serialization(buf.data(), pkt_size);
#else
            msg.write_serialization(buf.data(), pkt_size, nullptr);
#endif
            REQUIRE(buf == EXPECTED);
        }
    }

    GIVEN("A sample DNS message with several single level compressions")
    {
        CaptureDNS msg;
        msg.id(0x6692);
        msg.type(CaptureDNS::RESPONSE);
        msg.add_query(
            CaptureDNS::query(
                CaptureDNS::encode_domain_name("sec2.apnic.com"),
                CaptureDNS::A,
                CaptureDNS::IN
                )
            );
        msg.add_authority(
            CaptureDNS::resource(
                CaptureDNS::encode_domain_name("com"),
                CaptureDNS::encode_domain_name("a.gtld-servers.net"),
                CaptureDNS::NS,
                CaptureDNS::IN,
                172800)
            );
        msg.add_authority(
            CaptureDNS::resource(
                CaptureDNS::encode_domain_name("com"),
                CaptureDNS::encode_domain_name("b.gtld-servers.net"),
                CaptureDNS::NS,
                CaptureDNS::IN,
                172800)
            );

        THEN("Message is serialised with compressed name")
        {
            std::vector<uint8_t> EXPECTED
                { 0x66,0x92,0x80,0x00,0x00,0x01,0x00,0x00,
                  0x00,0x02,0x00,0x00,0x04,0x73,0x65,0x63,
                  0x32,0x05,0x61,0x70,0x6e,0x69,0x63,0x03,
                  0x63,0x6f,0x6d,0x00,0x00,0x01,0x00,0x01,
                  0xc0,0x17,0x00,0x02,0x00,0x01,0x00,0x02,
                  0xa3,0x00,0x00,0x14,0x01,0x61,0x0c,0x67,
                  0x74,0x6c,0x64,0x2d,0x73,0x65,0x72,0x76,
                  0x65,0x72,0x73,0x03,0x6e,0x65,0x74,0x00,
                  0xc0,0x17,0x00,0x02,0x00,0x01,0x00,0x02,
                  0xa3,0x00,0x00,0x04,0x01,0x62,0xc0,0x2e
                };
            uint32_t pkt_size = msg.header_size();
            REQUIRE(pkt_size == EXPECTED.size());

            std::vector<uint8_t> buf(pkt_size);
#ifdef HAVE_LIBTINS4
            msg.write_serialization(buf.data(), pkt_size);
#else
            msg.write_serialization(buf.data(), pkt_size, nullptr);
#endif
            REQUIRE(buf == EXPECTED);
        }
    }

    GIVEN("A sample DNS message with two level compressions")
    {
        CaptureDNS msg;
        msg.id(0x6692);
        msg.type(CaptureDNS::RESPONSE);
        msg.add_query(
            CaptureDNS::query(
                CaptureDNS::encode_domain_name("sec2.apnic.net"),
                CaptureDNS::A,
                CaptureDNS::IN
                )
            );
        msg.add_authority(
            CaptureDNS::resource(
                CaptureDNS::encode_domain_name("net"),
                CaptureDNS::encode_domain_name("a.gtld-servers.net"),
                CaptureDNS::NS,
                CaptureDNS::IN,
                172800)
            );
        msg.add_authority(
            CaptureDNS::resource(
                CaptureDNS::encode_domain_name("net"),
                CaptureDNS::encode_domain_name("b.gtld-servers.net"),
                CaptureDNS::NS,
                CaptureDNS::IN,
                172800)
            );

        THEN("Message is serialised with compressed name")
        {
            std::vector<uint8_t> EXPECTED
                { 0x66,0x92,0x80,0x00,0x00,0x01,0x00,0x00,
                  0x00,0x02,0x00,0x00,0x04,0x73,0x65,0x63,
                  0x32,0x05,0x61,0x70,0x6e,0x69,0x63,0x03,
                  0x6e,0x65,0x74,0x00,0x00,0x01,0x00,0x01,
                  0xc0,0x17,0x00,0x02,0x00,0x01,0x00,0x02,
                  0xa3,0x00,0x00,0x11,0x01,0x61,0x0c,0x67,
                  0x74,0x6c,0x64,0x2d,0x73,0x65,0x72,0x76,
                  0x65,0x72,0x73,0xc0,0x17,0xc0,0x17,0x00,
                  0x02,0x00,0x01,0x00,0x02,0xa3,0x00,0x00,
                  0x04,0x01,0x62,0xc0,0x2e
                };
            uint32_t pkt_size = msg.header_size();
            REQUIRE(pkt_size == EXPECTED.size());

            std::vector<uint8_t> buf(pkt_size);
#ifdef HAVE_LIBTINS4
            msg.write_serialization(buf.data(), pkt_size);
#else
            msg.write_serialization(buf.data(), pkt_size, nullptr);
#endif
            REQUIRE(buf == EXPECTED);
        }
    }
}

SCENARIO("DNS messages with compressed labels in RRs", "[dnspacket]")
{
    GIVEN("A sample MX message")
    {
        std::vector<uint8_t> MX
            { 0x2c,0x0a,0x81,0x80,0x00,0x01,0x00,0x01,
              0x00,0x05,0x00,0x03,0x05,0x6c,0x75,0x6e,
              0x63,0x68,0x03,0x6f,0x72,0x67,0x02,0x75,
              0x6b,0x00,0x00,0x0f,0x00,0x01,0xc0,0x0c,
              0x00,0x0f,0x00,0x01,0x00,0x00,0x0e,0x08,
              0x00,0x09,0x00,0x00,0x04,0x6d,0x61,0x69,
              0x6c,0xc0,0x0c,0xc0,0x0c,0x00,0x02,0x00,
              0x01,0x00,0x01,0x4d,0x7b,0x00,0x10,0x04,
              0x64,0x6e,0x73,0x31,0x05,0x6d,0x74,0x67,
              0x73,0x79,0x02,0x63,0x6f,0xc0,0x16,0xc0,
              0x0c,0x00,0x02,0x00,0x01,0x00,0x01,0x4d,
              0x7b,0x00,0x07,0x04,0x64,0x6e,0x73,0x32,
              0xc0,0x44,0xc0,0x0c,0x00,0x02,0x00,0x01,
              0x00,0x01,0x4d,0x7b,0x00,0x10,0x04,0x64,
              0x6e,0x73,0x34,0x05,0x6d,0x74,0x67,0x73,
              0x79,0x03,0x63,0x6f,0x6d,0x00,0xc0,0x0c,
              0x00,0x02,0x00,0x01,0x00,0x01,0x4d,0x7b,
              0x00,0x07,0x04,0x64,0x6e,0x73,0x30,0xc0,
              0x44,0xc0,0x0c,0x00,0x02,0x00,0x01,0x00,
              0x01,0x4d,0x7b,0x00,0x07,0x04,0x64,0x6e,
              0x73,0x33,0xc0,0x73,0xc0,0x2c,0x00,0x01,
              0x00,0x01,0x00,0x00,0x0a,0x0b,0x00,0x04,
              0xd5,0x8a,0x65,0x89,0xc0,0x2c,0x00,0x1c,
              0x00,0x01,0x00,0x00,0x0e,0x08,0x00,0x10,
              0x20,0x01,0x41,0xc8,0x00,0x51,0x01,0x89,
              0xfe,0xff,0x00,0xff,0xfe,0x00,0x0b,0x1c,
              0x00,0x00,0x29,0x10,0x00,0x00,0x00,0x00,
              0x00,0x00,0x00
            };
        CaptureDNS msg(MX.data(), MX.size());

        THEN("Domain in answer is expanded")
        {
            REQUIRE(msg.answers_count() == 1);
            REQUIRE(msg.answers().front().query_type() == CaptureDNS::MX);
            byte_string data = msg.answers().front().data();
            REQUIRE(data.size() == 21);
            byte_string label = data.substr(2);
            REQUIRE(CaptureDNS::decode_domain_name(label) == "mail.lunch.org.uk");
        }
    }

    GIVEN("A sample SRV message")
    {
        std::vector<uint8_t> SRV
            { 0xcc,0xdc,0x81,0x80,0x00,0x01,0x00,0x01,
              0x00,0x05,0x00,0x03,0x05,0x5f,0x69,0x6d,
              0x61,0x70,0x04,0x5f,0x74,0x63,0x70,0x05,
              0x6c,0x75,0x6e,0x63,0x68,0x03,0x6f,0x72,
              0x67,0x02,0x75,0x6b,0x00,0x00,0x21,0x00,
              0x01,0xc0,0x0c,0x00,0x21,0x00,0x01,0x00,
              0x00,0x0d,0xfd,0x00,0x19,0x00,0x00,0x00,
              0x01,0x00,0x8f,0x04,0x6d,0x61,0x69,0x6c,
              0x05,0x6c,0x75,0x6e,0x63,0x68,0x03,0x6f,
              0x72,0x67,0x02,0x75,0x6b,0x00,0xc0,0x17,
              0x00,0x02,0x00,0x01,0x00,0x01,0x47,0xbf,
              0x00,0x10,0x04,0x64,0x6e,0x73,0x30,0x05,
              0x6d,0x74,0x67,0x73,0x79,0x02,0x63,0x6f,
              0xc0,0x21,0xc0,0x17,0x00,0x02,0x00,0x01,
              0x00,0x01,0x47,0xbf,0x00,0x10,0x04,0x64,
              0x6e,0x73,0x34,0x05,0x6d,0x74,0x67,0x73,
              0x79,0x03,0x63,0x6f,0x6d,0x00,0xc0,0x17,
              0x00,0x02,0x00,0x01,0x00,0x01,0x47,0xbf,
              0x00,0x07,0x04,0x64,0x6e,0x73,0x31,0xc0,
              0x5f,0xc0,0x17,0x00,0x02,0x00,0x01,0x00,
              0x01,0x47,0xbf,0x00,0x07,0x04,0x64,0x6e,
              0x73,0x32,0xc0,0x5f,0xc0,0x17,0x00,0x02,
              0x00,0x01,0x00,0x01,0x47,0xbf,0x00,0x07,
              0x04,0x64,0x6e,0x73,0x33,0xc0,0x7b,0x04,
              0x6d,0x61,0x69,0x6c,0xc0,0x17,0x00,0x01,
              0x00,0x01,0x00,0x00,0x04,0x4f,0x00,0x04,
              0xd5,0x8a,0x65,0x89,0xc0,0xbf,0x00,0x1c,
              0x00,0x01,0x00,0x00,0x08,0x4c,0x00,0x10,
              0x20,0x01,0x41,0xc8,0x00,0x51,0x01,0x89,
              0xfe,0xff,0x00,0xff,0xfe,0x00,0x0b,0x1c,
              0x00,0x00,0x29,0x10,0x00,0x00,0x00,0x00,
              0x00,0x00,0x00
            };
        CaptureDNS msg(SRV.data(), SRV.size());

        THEN("Domain in answer is expanded")
        {
            REQUIRE(msg.answers_count() == 1);
            REQUIRE(msg.answers().front().query_type() == CaptureDNS::SRV);
            byte_string data = msg.answers().front().data();
            REQUIRE(data.size() == 25);
            byte_string label = data.substr(6);
            REQUIRE(CaptureDNS::decode_domain_name(label) == "mail.lunch.org.uk");
        }
    }
}

SCENARIO("DNS messages with EDNS0 options", "[dnspacket]")
{
    GIVEN("A sample message with EDNS0")
    {
        std::vector<uint8_t> EDNS0
          { 0x6c,0xac,0x01,0x00,0x00,0x01,0x00,0x00,
            0x00,0x00,0x00,0x01,0x09,0x67,0x65,0x74,
            0x64,0x6e,0x73,0x61,0x70,0x69,0x03,0x6e,
            0x65,0x74,0x00,0x00,0x1c,0x00,0x01,0x00,
            0x00,0x29,0x05,0x98,0x00,0x00,0x00,0x00,
            0x00,0x20,0x00,0x0a,0x00,0x08,0x01,0x02,
            0x03,0x04,0x05,0x06,0x07,0x08,0x00,0x0a,
            0x00,0x08,0x40,0x4b,0x9f,0x4d,0x1f,0x2b,
            0xe9,0x49,0x00,0x08,0x00,0x04,0x00,0x01,
            0x00,0x00
            };
        CaptureDNS msg(EDNS0.data(), EDNS0.size());

        THEN("EDNS0 options are extracted")
        {
            REQUIRE(msg.additional_count() == 1);
            auto edns0 = msg.edns0();
            REQUIRE(edns0);
            REQUIRE_FALSE(edns0->do_bit());
            REQUIRE(edns0->extended_rcode() == 0);
            REQUIRE(edns0->edns_version() == 0);
            REQUIRE(edns0->udp_payload_size() == 1432);
            CaptureDNS::EDNS0::options_type options = edns0->options();
            REQUIRE(options.size() == 3);
            int count = 0;
            for (const auto& o : options)
            {
                switch(count)
                {
                case 0:
                case 1:
                    REQUIRE(o.code() == CaptureDNS::COOKIE);
                    break;

                case 2:
                    REQUIRE(o.code() == CaptureDNS::CLIENT_SUBNET);
                    break;
                }
                ++count;
            }
        }
    }
}
