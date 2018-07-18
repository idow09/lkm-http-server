#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>

#define PTCP_WATCH_PORT     80  /* HTTP port */

static struct nf_hook_ops nfho;

static unsigned int ptcp_hook_func(const struct nf_hook_ops *ops,
                                   struct sk_buff *skb,
                                   const struct net_device *in,
                                   const struct net_device *out,
                                   int (*okfn)(struct sk_buff *))
{
    struct iphdr *iph;          /* IPv4 header */
    struct tcphdr *tcph;        /* TCP header */
    u16 sport, dport;           /* Source and destination ports */
    u32 saddr, daddr;           /* Source and destination addresses */
    unsigned char *user_data;   /* TCP data begin pointer */
    unsigned char *tail;        /* TCP data end pointer */
    unsigned char *it;          /* TCP data iterator */

    /* Network packet is empty, seems like some problem occurred. Skip it */
    if (!skb)
        return NF_ACCEPT;

    iph = ip_hdr(skb);          /* get IP header */

    /* Skip if it's not TCP packet */
    if (iph->protocol != IPPROTO_TCP)
        return NF_ACCEPT;

    tcph = tcp_hdr(skb);        /* get TCP header */

    /* Convert network endianness to host endiannes */
    saddr = ntohl(iph->saddr);
    daddr = ntohl(iph->daddr);
    sport = ntohs(tcph->source);
    dport = ntohs(tcph->dest);

    /* Watch only port of interest */
    if (sport != PTCP_WATCH_PORT)
        return NF_ACCEPT;

    /* Calculate pointers for begin and end of TCP packet data */
    user_data = (unsigned char *)((unsigned char *)tcph + (tcph->doff * 4));
    tail = skb_tail_pointer(skb);

    /* ----- Print all needed information from received TCP packet ------ */

    /* Show only HTTP packets */
    if (user_data[0] != 'H' || user_data[1] != 'T' || user_data[2] != 'T' ||
            user_data[3] != 'P') {
        return NF_ACCEPT;
    }

    /* Print packet route */
    pr_debug("print_tcp (http): %pI4h:%d -> %pI4h:%d\n", &saddr, sport,
                              &daddr, dport);

    /* Print TCP packet data (payload) */
/*    pr_debug("print_tcp: data:\n");
    for (it = user_data; it != tail; ++it) {
        char c = *(char *)it;

        if (c == '\0')
            break;

        printk("%c", c);
    }
    printk("\n\n");
*/
    return NF_ACCEPT;
}

static int __init ptcp_init(void)
{
    int res;

    nfho.hook = (nf_hookfn *)ptcp_hook_func;    /* hook function */
    nfho.hooknum = NF_INET_PRE_ROUTING;         /* received packets */
    nfho.pf = PF_INET;                          /* IPv4 */
    nfho.priority = NF_IP_PRI_FIRST;            /* max hook priority */

    res = nf_register_hook(&nfho);
    if (res < 0) {
        pr_err("print_tcp: error in nf_register_hook()\n");
        return res;
    }

    pr_debug("print_tcp: loaded\n");
    return 0;
}

static void __exit ptcp_exit(void)
{
    nf_unregister_hook(&nfho);
    pr_debug("print_tcp: unloaded\n");
}

module_init(ptcp_init);
module_exit(ptcp_exit);

MODULE_AUTHOR("Sam Protsenko");
MODULE_DESCRIPTION("Module for printing TCP packet data");
MODULE_LICENSE("GPL");

