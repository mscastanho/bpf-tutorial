Vagrant.configure("2") do |config|
    config.vm.box = "mscastanho/minicurso-ebpf"
    config.vm.box_version = "1.0"
    config.vm.define "sbrc2019" do |dsfc1|
    end

    # Private network for inter-vm communication
    config.vm.network "private_network", ip: "10.0.0.2"

    config.vm.synced_folder ".", "/vagrant", disabled: true

    config.vm.provider "virtualbox" do |vb|
        vb.name = "#{config.vm.hostname}"
        vb.gui = false
        vb.memory = "2048"
    end

    config.vm.provision "ansible" do |ansible|
        ansible.verbose = "vv"
        ansible.playbook = "./ansible/provision.yml"
        ansible.extra_vars = { ansible_python_interpreter:"/usr/bin/python3" }
    end
end