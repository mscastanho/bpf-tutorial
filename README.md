# Tutorial sobre extended Berkeley Packet Filter (eBPF)

Este repositório contém o material utilizado durante a realização do Minicurso sobre eBPF e XDP no [SBRC2019](http://sbrc2019.sbc.org.br/).

## Máquina virtual

Uma máquina virtual foi especialmente desenvolvida para este tutorial, contendo todo o código e ferramentas necessárias para a execução de cada passo.

- [Download VirtualBox VM](https://drive.google.com/open?id=1jFH4pE9oPLJbL0XtZE0pYylxfRITV8-i) (user: *ebpf*, senha: *ebpf*)

A máquina virtual está equipada com os seguintes itens:
- kernel v5.0.0
- iproute2 201903??
- llvm 6.0
- bpftool
    
Além disso, o diretório `/home/ebpf` apresenta cópias locais dos seguintes repositórios:
- [Linux kernel net-next](https://git.kernel.org/pub/scm/linux/kernel/git/davem/net-next.git)
- [BPFabric](https://github.com/UofG-netlab/BPFabric)
- [iproute2](https://git.kernel.org/pub/scm/network/iproute2/iproute2-next.git)
- [prototype-kernel](https://github.com/netoptimizer/prototype-kernel.git)

Além disso, a máquina foi configurada com 2 interfaces de rede, uma no modo NAT (acesso à internet) e outra no modo HostOnly (acesso por SSH). Após importar a máquina, certifique-se de que uma rede HostOnly `vboxnet0` está criada no VirtualBox, sob o menu File > Host Network Manager.