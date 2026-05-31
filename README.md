FetchOS is a Debian-based distro with a dog-themed UX layer — custom package manager (woof), system fetch tool (dogefetch), and a virtual pet that decays over time if you ignore it.(FETCHOS also has many built in custom coded tools)🐾
Here are the run instructions for QUEM, however I recommend Oracle ViurtialBox for a simpler setup.

``` 
qemu-img create -f qcow2 fetchos_drive.qcow2 20G

qemu-system-x86_64 -m 1024 -smp 2 -cpu host \
  -boot d \
  -drive file=fetchos_drive.qcow2,format=qcow2 \
  -cdrom FetchOS-0.3.iso \
  -net nic,model=virtio -net user \
  -enable-kvm

```
FechOS is pretty lightwheigt and can run on pc's with as low as 512 mb but I would recommend 1gb.
<img width="723" height="478" alt="image" src="https://github.com/user-attachments/assets/b1a55852-4491-4937-bb3a-357a62a56a63" />
