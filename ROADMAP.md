## Exemplo xdp1

- Mostrar código rodando

- Mostrar entradas no mapa com o comando `bpftool`

- Alterar o programa para deixar os pacotes passarem

    ```c
    int rc = XDP_DROP; => int rc = XDP_PASS;
    ```

## Exemplo metadata

- Arquivo `samples/bpf/xdp2skb_meta_kern.c`

- Adicionar `bpf_custom_printk`:

    ```c
    // Nicer way to call bpf_trace_printk()
    #define bpf_custom_printk(fmt, ...)                     \
            ({                                              \
                char ____fmt[] = fmt;                       \
                bpf_trace_printk(____fmt, sizeof(____fmt),  \
                        ##__VA_ARGS__);                     \
            })

    bpf_custom_printk("[XDP] metadado = %d\n",meta->mark);

    bpf_custom_printk("[TC] metadado = %d\n",meta->mark);
    ```

- Carregar código e mostrar erro do verificador

- Adicionar licença

    ```c
    char _license[] SEC("license") = "GPL";
    ```

- Mostrar output no arquivo `/sys/kernel/debug/tracing/trace_pipe`