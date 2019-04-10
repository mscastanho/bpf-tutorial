# Script de configuração de ambiente para eBPF

Ao invés de usar a VM disponibilizada para o minicurso, também é possível provisionar uma outra máquina com todos as dependências necessárias. Para isso disponibilizamos o script `provision.yml`, que pode ser utilizando com a ferramenta [ansible](https://github.com/ansible/ansible).

**ATENÇÃO**: Este script instala a versão 5.0 do kernel do Linux, além de diversas outras dependências. Aconselhamos a sua utilização apenas em máquinas de teste/desenvolvimento.

Essa ferramenta se conecta por SSH com uma máquina remota e efetua todos os passos de instalação especificados no script. Em sua máquina local, execute:

    ansible-playbook -i "<MACHINE-IP>," -u <USER> -k -K -e "ansible_python_interpreter=/usr/bin/python3" provision.yml

Onde `<MACHINE-IP>` e `<USER>` devem ser substituídos pelos valores correspondentes à máquina alvo. O ansible pedirá a senha de acesso SSH e a senha para executar operações como super usuário na máquina remota.