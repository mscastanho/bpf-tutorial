# Tutorial sobre extended Berkeley Packet Filter (eBPF)

Este repositório contém o material utilizado durante a realização do Minicurso sobre eBPF e XDP no [SBRC2019](http://sbrc2019.sbc.org.br/).

## Conteúdo

O conteúdo deste repositório está dividido da seguinte forma:
- `ansible/`: script ansible usado para instalar as dependências necessárias durante a criação da VM
- `exemplos/`: exemplos de programas eBPF utilizados durante o minicurso
- `headers/`: headers necessários para compilação dos programas
- `images/`: imagens utilizadas neste README
- `slides.pdf`: slides com o conteúdo teórico utilizado na apresentação

## Máquina virtual

Uma máquina virtual foi especialmente desenvolvida para este tutorial, contendo todo o código e ferramentas necessárias para a execução de cada passo.

- [Download VirtualBox VM](http://dcc.ufmg.br/~matheus.castanho/minicurso-ebpf-sbrc2019.rar) (user: *ebpf*, senha: *ebpf*)

A máquina virtual está equipada com os seguintes itens:
- kernel v5.0.0
- iproute2-ss190319
- llvm 6.0.0
- bpftool
    
Além de uma cópia deste repositório, o diretório `/home/ebpf` apresenta cópias locais dos seguintes projetos:
- [Linux kernel net-next](https://git.kernel.org/pub/scm/linux/kernel/git/davem/net-next.git)
- [BPFabric](https://github.com/UofG-netlab/BPFabric)
- [iproute2](https://git.kernel.org/pub/scm/network/iproute2/iproute2-next.git)
- [prototype-kernel](https://github.com/netoptimizer/prototype-kernel.git)

## Importando a máquina virtual

Os passos a seguir foram testados com o VirtualBox 5.2.18 no Ubuntu.

Após fazer o download da imagem, descompacte o arquivo `.rar`. O arquivo resultante será `minicurso-ebpf-sbrc2019.ova`.

Para importar a VM, acesse o menu `Arquivo > Importar Appliance` e selecione o arquivo `.ova` descompactado.

<p align="center">
    <img src="./images/vbox-import.png" alt="Importar Appliance" width="500" align="middle">
</p>

Em seguida, o VirtualBox permitirá modificações na especificação da máquina, como quantidade de memória RAM (padrão: 4GB).

<p align="center">
    <img src="./images/vbox-import-2.png" alt="Importar Appliance" width="500" align="middle">
</p>

A máquina foi configurada com 2 interfaces de rede, uma no modo NAT (`eth0` - acesso à internet) e outra no modo HostOnly (`eth1` - acesso por SSH). Após a importação, certifique-se de que uma rede HostOnly `vboxnet0` está criada no VirtualBox, sob o menu `Arquivo > Host Network Manager`.

<p align="center">
    <img src="./images/vbox-net.png" alt="Importar Appliance" width="500" align="middle">
</p>

## Compilando os exemplos do kernel

O código fonte do kernel apresenta diversos programas de exemplo, disponíveis nos seguintes diretórios:
    - `samples/bpf`
    - `tools/testing/selftests/bpf`

Neste minicurso apresentaremos dois exemplos presentes na pasta `samples/bpf`. Para compilá-los, execute os seguintes comandos:

    cd ~/net-next/
    make headers_install
    make samples/bpf/

## Compilando os exemplos locais

Os exemplos fornecidos neste repositório na pasta `exemplos/` estão acompanhados de um Makefile. Para compilá-los:

    cd exemplos/
    make

**OBS**: As dependências necessárias para a compilação já estão instaladas na máquina virtual, portanto recomendamos compilar os exemplos naquele ambiente.

## Exemplo 1: Drop World!

Local: `./exemplos/dropworld.c`

Esse exemplo representa um dos programas mais simples possíveis. A sua funcionalidade é simplesmente descartar todos os pacotes recebidos.

Para compilá-lo, utilize o comando `make`:

    cd ./exemplos/
    make

Em seguida, o programa pode ser carregado utilizado a ferramenta `ip`:

    sudo ip -force link set dev eth0 xdp obj dropworld.o sec .text

O argumento `.text`se refere à seção ELF no qual o programa está localizado. Conferir os slides para mais detalhes.

Podemos conferir o status do programa utilizando o seguinte comando:

    ip link show eth0

Output:

    ebpf@sbrc2019:~/bpf-tutorial/exemplos$ ip link show eth0
    2: eth0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 xdpgeneric qdisc fq_codel state UP mode DEFAULT group default qlen 1000
    link/ether 08:00:27:58:07:42 brd ff:ff:ff:ff:ff:ff
    prog/xdp id 19

Para removermos o programa, basta executar:

    sudo ip link set dev eth0 xdp off

Após a remoção o status da interface será o seguinte:

    ebpf@sbrc2019:~/bpf-tutorial/exemplos$ ip link show eth0
    2: eth0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc fq_codel state UP mode DEFAULT group default qlen 1000
        link/ether 08:00:27:58:07:42 brd ff:ff:ff:ff:ff:ff

Como nesse caso a interface `eth0` é utilizada para acesso à Internet, o descarte de pacotes recebidos por essa interface efetivamente cortará o acesso à web.

Em um terminal separado, inicie um processo para "pingar" um domínio na internet:

    ping google.com

Em seguida, carregue o programa `dropworld.o` na interface eth0 e veja como a respostas ao ping serão interrompidas. Isso ocorrerá pois todas mensagens de respostas enviadas à interface `eth0` serão descartadas pelo programa carregado.

**Extra**: modifique o arquivo `dropworld.c` e modifique o valor de retorno de `XDP_DROP` para `XDP_PASS`, compile e repita o processo de carregamento. Nesse caso, perceba que as repostas ao ping ainda serão recebidas, de forma que esse novo programa é efetivamente uma operação vazia, que simples recebe e passa os pacotes para cima na pilha do kernel.

## Exemplo 2: Filtragem de pacotes por porta TCP

Local: `./exemplos/portfilter.c`

Este exemplo analisa os pacotes recebidos em uma interface e descarta pacotes com o protocolo HTTP. O descarte é feito por meio da análise dos campos de porta de origem e destino do cabeçalho TCP. Pacotes nos quais um desses valores é igual a 80 são descartados.

Similarmente ao exemplo anterior, compile o programa:

    cd ./exemplos/
    make

Antes de carregar o programa, teste o acesso a uma página web:

    curl https://www.google.com

A saída desse comando deve ser um print do código HTML da página em questão.

Carregue utilizando a ferramenta `ip`:

    sudo ip -force link set dev eth0 xdp obj portfilter.o sec pfilter

Agora tente novamente acessar a mesma página:

    curl https://www.google.com

Por conta do programa *portfilter.o* o os pacotes serão descartados assim que chegarem à interface `eth0`, impedindo o acesso.

**Extra**: Modifique o arquivo *portfilter.c* para descartar pacotes ICMP (utilizados pela ferramenta `ping`).

## Exemplo 3: Interação com mapas e espaço de usuário

Local: `xdp1_kern.c` e `xdp1_user.c` localizados no diretório `samples/bpf/` do código fonte do kernel (`~/net-next/samples/bpf/` na VM).

Este exemplo demonstra a utilização de mapas em programas eBPF e como fazer a interação com o espaço de usuário. O programa em `xdp1_kern.c` extrai de cada pacote recebido o número do protocolo de camada 4 utilizado (TCP = 6, UDP = 17, ICMP = 1, etc), atualiza um contador para cada protocolo e em seguida descarta o pacote. Esses valores são salvos no mapa `rxcnt`, que é consultado pelo programa `xdp1_user.c`, que executa em espaço de usuário. Por meio do uso de um mapa, os dois programas (um em kernel e outro em espaço de usuário) são capazes de trocar informação.

Para compilar os programas, siga as instruções dadas anteriormente sobre como compilar programas de exemplo do kernel do Linux.

Diferentemente dos exemplos anteriores, aqui o programa eBPF é carregado no kernel pelo programa `xdp_user.c`, em espaço de usuário, sem a necessidade do uso da ferramenta `ip`.

Após a compilação, o diretório `samples/bpf/` conterá o arquivo executável `xdp1`, gerado após a compilação de `xdp_user.c`.

    ebpf@sbrc2019:~/net-next/samples/bpf$ ./xdp1 
    usage: xdp1 [OPTS] IFACE

    OPTS:
        -S    use skb-mode
        -N    enforce native mode

Para carregar o programa na interface `eth0` basta passá-la como parâmetro:

    ./xdp1 eth0

O programa entrará em um loop infinito, imprimindo o número de pacotes recebidos por número de protocolo até o momento.

    ebpf@sbrc2019:~/net-next/samples/bpf$ sudo ./xdp1 eth0
    proto 17:          1 pkt/s
    proto 17:          1 pkt/s
    proto 17:          1 pkt/s
    proto 0:          1 pkt/s
    proto 17:          1 pkt/s

Em um outro terminal, faça alguma requisição utilizando as ferramentas `ping`, `curl`, `wget` e afins, para fazer com que pacotes passem pela interface.

É possível analisar o conteúdo do mapa utilizando a ferramenta `bpftool`, já compilada e instalada na VM fornecida. Para isso, primeiramente verificamos os programas eBPF carregados no sistema:

    sudo bpftool prog show 

Output:

    ebpf@sbrc2019:~$ sudo bpftool prog show
    2: cgroup_skb  tag 7be49e3934a125ba  gpl
            loaded_at 2019-04-23T12:24:29-0400  uid 0
            xlated 296B  jited 229B  memlock 4096B  map_ids 2,3
    3: cgroup_skb  tag 2a142ef67aaad174  gpl
            loaded_at 2019-04-23T12:24:29-0400  uid 0
            xlated 296B  jited 229B  memlock 4096B  map_ids 2,3
    4: cgroup_skb  tag 7be49e3934a125ba  gpl
            loaded_at 2019-04-23T12:24:29-0400  uid 0
            xlated 296B  jited 229B  memlock 4096B  map_ids 4,5
    5: cgroup_skb  tag 2a142ef67aaad174  gpl
            loaded_at 2019-04-23T12:24:29-0400  uid 0
            xlated 296B  jited 229B  memlock 4096B  map_ids 4,5
    6: cgroup_skb  tag 7be49e3934a125ba  gpl
            loaded_at 2019-04-23T12:24:35-0400  uid 0
            xlated 296B  jited 229B  memlock 4096B  map_ids 6,7
    7: cgroup_skb  tag 2a142ef67aaad174  gpl
            loaded_at 2019-04-23T12:24:35-0400  uid 0
            xlated 296B  jited 229B  memlock 4096B  map_ids 6,7
    28: xdp  name xdp_prog1  tag 539ec6ce11b52f98  gpl
            loaded_at 2019-04-23T14:34:06-0400  uid 0
            xlated 488B  jited 336B  memlock 4096B  map_ids 14

O último programa listado corresponde ao programa XDP carregado por `xdp1`. A saída também indica que ele apresenta um mapa com o id 14. Podemos utilizar esse valor para consultar o conteúdo do mapa:

    sudo bpftool map dump id 14

Output:

    ebpf@sbrc2019:~$ sudo bpftool map dump id 14
    key:
    00 00 00 00
    value (CPU 00): 4b 00 00 00 00 00 00 00
    key:
    01 00 00 00
    value (CPU 00): 00 00 00 00 00 00 00 00
    key:
    02 00 00 00
    value (CPU 00): 00 00 00 00 00 00 00 00
    ... (restante da saída omitido)

O mapa utilizado é do tipo `BPF_MAP_TYPE_PERCPU_ARRAY`. Como o nome indica, ele apresenta um vetor por CPU utilizada. Na declaração do mapa, o número de elementos foi definido como `256`, portanto a saída do comando `bpftool` acima mostra 256 entradas correspondentes à CPU 0, única da VM.

**Extra**: Alterar o programa para deixar os pacotes passarem, ao invés de serem descartados. Além disso, mudar o tipo do mapa para `BPF_MAP_TYPE_HASH` e conferir o output com a ferramenta `bpftool`.

## Exemplo 4: Interação entre camadas XDP e TC

Local: `linux/samples/bpf/`: arquivos `xdp2skb_meta_kern.c` e `xdp2skb_meta.sh`