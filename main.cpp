#include "Rejex.hpp"
#include "valid.hpp"
#include "invalid.hpp"
#include <iostream>
#include <string>

void test(Rejex &r, const std::string &s)
{
	if (r.match(s))
	{
		std::cout << "MATCH: " << s << std::endl;
	}
	else
	{
		std::cout << "ERROR: " << s << std::endl;
	}
}

void init_ip6(Rejex &r)
{
    Rejex ip4byte;
    ip4byte.exp(Rejex().all("2").all("5").any("012345")).jmp();
    ip4byte.exp(Rejex().all("2").any("01234").digit()).jmp();
    ip4byte.exp(Rejex().any("01").digit(2, 2)).jmp();
    ip4byte.digit(1, 2);

    Rejex ip4addr;
    ip4addr.exp(ip4byte).all(".").exp(ip4byte).all(".").exp(ip4byte).all(".").exp(ip4byte);

    Rejex ip6pre;
    ip6pre.xdigit(1, 4).all(":");
    Rejex ip6suf;
    ip6suf.all(":").xdigit(1, 4);

    Rejex ip6pre0;
    ip6pre0.all("0", 1, 4).all(":");
    Rejex ip6suf0;
    ip6suf0.all(":").all("0", 1, 4);
    Rejex ip6ffff;
    ip6ffff.all(":").any("fF", 4, 4).all(":");

    // IPv6 addresses
    Rejex ip6addr;

    // IPv4 mapped addresses 0:0:0:0:0:ffff:a.b.c.d
    ip6addr.exp(Rejex().exp(ip6pre0, 4, 4).any("0", 1, 4).exp(ip6ffff).exp(ip4addr)).jmp();
    ip6addr.exp(Rejex().exp(ip6pre0, 1, 4).exp(ip6ffff).exp(ip4addr)).jmp();
    ip6addr.exp(Rejex().any(":").exp(ip6suf0, 0, 4).exp(ip6ffff).exp(ip4addr)).jmp();

    // IPv4 translated addresses 0:0:0:0:ffff:0:a.b.c.d
    ip6addr.exp(Rejex().exp(ip6pre0, 3, 3).any("0", 1, 4).exp(ip6ffff).exp(ip6pre0).exp(ip4addr)).jmp();
    ip6addr.exp(Rejex().exp(ip6pre0, 1, 3).exp(ip6ffff).exp(ip6pre0).exp(ip4addr)).jmp();
    ip6addr.exp(Rejex().any(":").exp(ip6suf0, 0, 3).exp(ip6ffff).exp(ip6pre0).exp(ip4addr)).jmp();

    // IPv4 standard addresses 0:0:0:0:0:0:0:0
    ip6addr.exp(Rejex().exp(ip6pre, 7, 7).xdigit(1, 4)).jmp();
    ip6addr.exp(Rejex().exp(ip6pre, 6, 6).exp(ip6suf, 1, 1)).jmp();
    ip6addr.exp(Rejex().exp(ip6pre, 5, 5).exp(ip6suf, 1, 2)).jmp();
    ip6addr.exp(Rejex().exp(ip6pre, 4, 4).exp(ip6suf, 1, 3)).jmp();
    ip6addr.exp(Rejex().exp(ip6pre, 3, 3).exp(ip6suf, 1, 4)).jmp();
    ip6addr.exp(Rejex().exp(ip6pre, 2, 2).exp(ip6suf, 1, 5)).jmp();
    ip6addr.exp(Rejex().exp(ip6pre, 1, 1).exp(ip6suf, 1, 6)).jmp();
    ip6addr.exp(Rejex().any(":").exp(ip6suf, 1, 7)).jmp();
    ip6addr.exp(Rejex().exp(ip6pre, 1, 7).any(":")).jmp();

    // special case ::
    ip6addr.all("::");

    r = Rejex().exp(ip4addr).jmp().exp(ip6addr);
}

int main()
{
	Rejex r;
	int i;

	init_ip6(r);
	for (i = 0; g_valid[i].size() != 0; i++)
		test(r, g_valid[i]);
	for (i = 0; g_invalid[i].size() != 0; i++)
		test(r, g_invalid[i]);

    Rejex t;

    t.digit(3, 3).jmp(1).digit(1, 2).con().all(",").digit(3, 3);

    std::cout << std::endl;
    test(t, "100,000");
    std::cout << t.tagged(1) << std::endl;
    test(t, "10,000");
    std::cout << t.tagged(1) << std::endl;
    test(t, "1,000");
    std::cout << t.tagged(1) << std::endl;

    Rejex a;
    a.all("abc", 0, 3);
    a.all(" ").any("def", 0, 3).any('g', 'i', 0, 3);
    a.any(" ").inv("jkl", 0, 3).inv('m', 'o', 0, 3);
    a.all("xyz").jmp();
    a.exp(a);

    test(a, "abcabcabc defghi mnojklabcabcabc defghi mnojkl");

	return (0);
}
