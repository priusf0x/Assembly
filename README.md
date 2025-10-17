<h1 align="left">Processor (DREC TASK)</h1>

###

<div>
  <img style="100%" src="https://capsule-render.vercel.app/api?type=waving&height=90&section=header&reversal=false&fontSize=70&fontColor=FFFFFF&fontAlign=50&fontAlignY=50&stroke=-&descSize=20&descAlign=50&descAlignY=50&theme=cobalt"  />
</div>

###

<h3 align="left">Work Example</h2>

###

![til](processor_cutted.gif)


###

|Command name| Description |
| --- | --- |
|jmp <instrucrion_number/<label_name:>| Default jump|
|ja <instrucrion_number/<label_name:>| Jump If Above|
|jae <instrucrion_number/<label_name:>| Jump If Above or Equal|
|jb <instrucrion_number/<label_name:>| Jump If Below|
|jbe <instrucrion_number/<label_name:>| Jump If Below or Equal|
|je <instrucrion_number/<label_name:>| Jump If Equal|
|jne <instrucrion_number/<label_name:>| Jump If Not Equal|
|call <label_name:>| Jump with pushing adress into stack|
|ret <label_name:>| returns to adress which is 

<p align="left">This is virtual processor with his own assembler included for running programms</p>

###
<h3 align="left">Installation</h3>

###

```bash
git clone https://github.com/priusf0x/Assembly
```

For building the project open installation directory and write in cmd:

```bash
make build 
```

Now you have 3 different options

```bash
./compiler.out <path-to-file(default_name = input_file.asm> <path-to-compiled-file(default_name = compiled.obj)>
```

```bash
./disassembler.out <path-to-file(default_name = compiled.obj> <path-to-dissassemvled-file(default_name = disassembled.asm)>
```

```bash
./processor.out <path-to-file(default_name = compiled.obj>
```

###

<h3 align="left">General commands</h2>

###

<h3 align="left">Memory controlling</h3>

###

|Command name| Description |
| --- | --- |
|push <integer>| Pushes integer into stack|
|push <register_name>| Pushes integer from register|
|push [register_name]| Pushes integer from memory cell with number which is written in register|
|pop <register_name>| Pops integer to register|
|pop [register_name]| Pops integer to memory cell with number which is written in register|

###

<h3 align="left">Ariphmetic operations</h3>

###

Ariphmeic operations with stack 

| add | sub | mul | div | sqrt |
|---|---|---|---|---|

###

<h3 align="left">User Interactions</h3>

###

|Command name| Description |
| --- | --- |
|draw| draw screen using size of pixel like integer size|
|drawb| optimized draw function|
|in| write user's input from cmd to stack|

###

<h3 align="left">Jumps/Call function</h3>

###

|Command name| Description |
| --- | --- |
|jmp <instrucrion_number/<label_name:>| Default jump|
|ja <instrucrion_number/<label_name:>| Jump If Above|
|jae <instrucrion_number/<label_name:>| Jump If Above or Equal|
|jb <instrucrion_number/<label_name:>| Jump If Below|
|jbe <instrucrion_number/<label_name:>| Jump If Below or Equal|
|je <instrucrion_number/<label_name:>| Jump If Equal|
|jne <instrucrion_number/<label_name:>| Jump If Not Equal|
|call <label_name:>| Jump with pushing adress into stack|
|ret <label_name:>| returns to adress which is written in stack|

<h4 align="left">Was created with</h4>

###

<div align="left">
  <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/c/c-original.svg" height="40" alt="c logo"  />
</div>

###
