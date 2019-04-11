# Tutorial sobre extended Berkeley Packet Filter (eBPF)

Este repositório contém o material utilizado durante a realização do Minicurso sobre eBPF e XDP no [SBRC2019](http://sbrc2019.sbc.org.br/).

## Dependências

 - `vagrant` ([Download](https://www.vagrantup.com/downloads.html))
 - VirtualBox

## Configurando o ambiente

Primeiramente, clone o este repositório:

    git clone https://github.com/mscastanho/bpf-tutorial.git

Uma máquina virtual foi especialmente desenvolvida para este tutorial, contendo todo o código e ferramentas necessárias para a execução de cada passo. Para utilizá-la, basta executar o seguinte comando:

    cd bpf-tutorial
    vagrant up

Esse comando fará o download automático da VM, além de sua instalação e configuração no VirtualBox. Ao fim da instalação, para acessar a VM basta executar:

    vagrant ssh