// license:BSD-3-Clause
// copyright-holders:Aaron Giles, Patrick Mackinlay

#ifndef MAME_CPU_MIPS_MIPS1_H
#define MAME_CPU_MIPS_MIPS1_H

#pragma once

class mips1core_device_base : public cpu_device
{
public:
	// device configuration
	void set_endianness(endianness_t endianness) { m_endianness = endianness; }
	void set_fpurev(u32 revision) { m_hasfpu = true; m_fpurev = revision; }

	// input lines
	template <unsigned Coprocessor> auto in_brcond() { return m_in_brcond[Coprocessor].bind(); }

protected:
	mips1core_device_base(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, u32 clock, u32 cpurev, size_t icache_size, size_t dcache_size);

	enum registers
	{
		// general purpose cpu registers
		MIPS1_R0, MIPS1_R1, MIPS1_R2, MIPS1_R3, MIPS1_R4, MIPS1_R5, MIPS1_R6, MIPS1_R7,
		MIPS1_R8, MIPS1_R9, MIPS1_R10, MIPS1_R11, MIPS1_R12, MIPS1_R13, MIPS1_R14, MIPS1_R15,
		MIPS1_R16, MIPS1_R17, MIPS1_R18, MIPS1_R19, MIPS1_R20, MIPS1_R21, MIPS1_R22, MIPS1_R23,
		MIPS1_R24, MIPS1_R25, MIPS1_R26, MIPS1_R27, MIPS1_R28, MIPS1_R29, MIPS1_R30, MIPS1_R31,

		// other cpu registers
		MIPS1_HI,
		MIPS1_LO,
		MIPS1_PC,

		// coprocessor 0 registers
		MIPS1_COP0_INDEX,    // reg 0, tlb only
		MIPS1_COP0_RANDOM,   // reg 1, tlb only
		MIPS1_COP0_ENTRYLO,  // reg 2, tlb only
		MIPS1_COP0_BUSCTRL,  // reg 2, r3041 only
		MIPS1_COP0_CONFIG,   // reg 3, r3041/r3071/r3081 only
		MIPS1_COP0_CONTEXT,  // reg 4, tlb only
		MIPS1_COP0_BADVADDR, // reg 8
		MIPS1_COP0_COUNT,    // reg 9, r3041 only
		MIPS1_COP0_ENTRYHI,  // reg 10, tlb only
		MIPS1_COP0_PORTSIZE, // reg 10, r3041 only
		MIPS1_COP0_COMPARE,  // reg 11, r3041 only
		MIPS1_COP0_SR,       // reg 12
		MIPS1_COP0_CAUSE,    // reg 13
		MIPS1_COP0_EPC,      // reg 14
		MIPS1_COP0_PRID,     // reg 15
	};

	enum exception : int
	{
		EXCEPTION_INTERRUPT = 0,
		EXCEPTION_TLBMOD    = 1,
		EXCEPTION_TLBLOAD   = 2,
		EXCEPTION_TLBSTORE  = 3,
		EXCEPTION_ADDRLOAD  = 4,
		EXCEPTION_ADDRSTORE = 5,
		EXCEPTION_BUSINST   = 6,
		EXCEPTION_BUSDATA   = 7,
		EXCEPTION_SYSCALL   = 8,
		EXCEPTION_BREAK     = 9,
		EXCEPTION_INVALIDOP = 10,
		EXCEPTION_BADCOP    = 11,
		EXCEPTION_OVERFLOW  = 12,
		EXCEPTION_TRAP      = 13,
	};

	enum cop0_reg : u8
	{
		COP0_Index    = 0,
		COP0_Random   = 1,
		COP0_EntryLo  = 2,
		COP0_BusCtrl  = 2,  // r3041 only
		COP0_Config   = 3,  // r3041/r3071/r3081 only
		COP0_Context  = 4,
		COP0_BadVAddr = 8,
		COP0_Count    = 9,  // r3041 only
		COP0_EntryHi  = 10,
		COP0_PortSize = 10, // r3041 only
		COP0_Compare  = 11, // r3041 only
		COP0_Status   = 12,
		COP0_Cause    = 13,
		COP0_EPC      = 14,
		COP0_PRId     = 15,
	};

	enum sr_mask : u32
	{
		SR_IEc   = 0x00000001,
		SR_KUc   = 0x00000002,
		SR_IEp   = 0x00000004,
		SR_KUp   = 0x00000008,
		SR_IEo   = 0x00000010,
		SR_KUo   = 0x00000020,
		SR_IMSW0 = 0x00000100,
		SR_IMSW1 = 0x00000200,
		SR_IMEX0 = 0x00000400,
		SR_IMEX1 = 0x00000800,
		SR_IMEX2 = 0x00001000,
		SR_IMEX3 = 0x00002000,
		SR_IMEX4 = 0x00004000,
		SR_IMEX5 = 0x00008000,
		SR_IsC   = 0x00010000,
		SR_SwC   = 0x00020000,
		SR_PZ    = 0x00040000,
		SR_CM    = 0x00080000,
		SR_PE    = 0x00100000,
		SR_TS    = 0x00200000,
		SR_BEV   = 0x00400000,
		SR_RE    = 0x02000000,
		SR_COP0  = 0x10000000,
		SR_COP1  = 0x20000000,
		SR_COP2  = 0x40000000,
		SR_COP3  = 0x80000000,
	};

	enum entryhi_mask : u32
	{
		EH_VPN  = 0xfffff000, // virtual page number
		EH_ASID = 0x00000fc0, // address space identifier
	};
	enum entrylo_mask : u32
	{
		EL_PFN = 0xfffff000, // physical frame
		EL_N   = 0x00000800, // noncacheable
		EL_D   = 0x00000400, // dirty
		EL_V   = 0x00000200, // valid
		EL_G   = 0x00000100, // global
	};

	// device_t overrides
	virtual void device_add_mconfig(machine_config &config) override;
	virtual void device_start() override;
	virtual void device_reset() override;

	// device_execute_interface overrides
	virtual u32 execute_min_cycles() const override { return 1; }
	virtual u32 execute_max_cycles() const override { return 40; }
	virtual u32 execute_input_lines() const override { return 6; }
	virtual void execute_run() override;
	virtual void execute_set_input(int inputnum, int state) override { set_irq_line(inputnum, state); }

	// device_memory_interface overrides
	virtual space_config_vector memory_space_config() const override;
	virtual bool memory_translate(int spacenum, int intention, offs_t &address) override;

	// device_disasm_interface overrides
	virtual std::unique_ptr<util::disasm_interface> create_disassembler() override;

	void icache_map(address_map &map);
	void dcache_map(address_map &map);

	// interrupts
	void generate_exception(int exception);
	void check_irqs();
	void set_irq_line(int irqline, int state);

	// cop0
	virtual u32 get_cop0_reg(int const index);
	void set_cop0_reg(int const index, u32 const data);
	virtual void handle_cop0(u32 const op);

	// cop1
	void set_cop1_creg(int const index, u32 const data);
	void handle_cop1(u32 const op);

	// generic coprocessor implementation
	template <unsigned Coprocessor> void handle_cop(u32 const op);
	template <unsigned Coprocessor> u32 get_cop_reg(int const index) { return m_cpr[Coprocessor][index]; }
	template <unsigned Coprocessor> void set_cop_reg(int const index, u32 const data) { m_cpr[Coprocessor][index] = data; }
	template <unsigned Coprocessor> u32 get_cop_creg(int const index) { return m_ccr[Coprocessor][index]; }
	template <unsigned Coprocessor> void set_cop_creg(int const index, u32 const data) { m_ccr[Coprocessor][index] = data; }

	// load/store left/right opcodes
	void lwl(u32 const op);
	void lwr(u32 const op);
	void swl(u32 const op);
	void swr(u32 const op);

	// memory accessors
	template <typename T, typename U> std::enable_if_t<std::is_convertible<U, std::function<void(T)>>::value, void> load(u32 program_address, U &&apply);
	template <typename T, typename U> std::enable_if_t<std::is_convertible<U, T>::value, void> store(u32 program_address, U data);
	bool fetch(u32 program_address, std::function<void(u32)> &&apply);

	// address spaces
	const address_space_config m_program_config_be;
	const address_space_config m_program_config_le;
	const address_space_config m_icache_config;
	const address_space_config m_dcache_config;

	int m_data_spacenum;

	// configuration
	u32 m_cpurev;
	bool m_hasfpu;
	u32 m_fpurev;
	endianness_t m_endianness;

	// core registers
	u32 m_pc;
	u32 m_nextpc;
	u32 m_hi;
	u32 m_lo;
	u32 m_r[32];

	// COP registers
	u32 m_cpr[4][32];
	u32 m_ccr[4][32];

	// internal stuff
	u32 m_ppc;
	int m_icount;

	// cache memory
	size_t const m_icache_size;
	size_t const m_dcache_size;

	// I/O
	devcb_read_line m_in_brcond[4];
};

class mips1_device_base : public mips1core_device_base
{
protected:
	mips1_device_base(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, u32 clock, u32 cpurev, size_t icache_size, size_t dcache_size);

	// device_t overrides
	virtual void device_start() override;
	virtual void device_reset() override;

	// device_memory_interface overrides
	virtual bool memory_translate(int spacenum, int intention, offs_t &address) override;

	virtual u32 get_cop0_reg(int idx) override;
	virtual void handle_cop0(u32 const op) override;

private:
	u64 m_reset_time;
	u32 m_tlb[64][2]; // 0 is hi, 1 is lo
};

class r2000_device : public mips1_device_base
{
public:
	r2000_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock, size_t icache_size = 0, size_t dcache_size = 0);
};

class r2000a_device : public mips1_device_base
{
public:
	r2000a_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock, size_t icache_size = 0, size_t dcache_size = 0);
};

class r3000_device : public mips1_device_base
{
public:
	r3000_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock, size_t icache_size = 0, size_t dcache_size = 0);
};

class r3000a_device : public mips1_device_base
{
public:
	r3000a_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock, size_t icache_size = 0, size_t dcache_size = 0);
};

class r3041_device : public mips1core_device_base
{
public:
	r3041_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock);

protected:
	virtual void device_start() override;
};

class r3051_device : public mips1core_device_base
{
public:
	r3051_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock);
};

class r3052_device : public mips1core_device_base
{
public:
	r3052_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock);
};

class r3052e_device : public mips1_device_base
{
public:
	r3052e_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock);
};

class r3071_device : public mips1core_device_base
{
public:
	r3071_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock, size_t icache_size = 16384, size_t dcache_size = 4096);
};

class r3081_device : public mips1core_device_base
{
public:
	r3081_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock, size_t icache_size = 16384, size_t dcache_size = 4096);
};

class iop_device : public mips1core_device_base
{
public:
	iop_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock);
};

DECLARE_DEVICE_TYPE(R2000,       r2000_device)
DECLARE_DEVICE_TYPE(R2000A,      r2000a_device)
DECLARE_DEVICE_TYPE(R3000,       r3000_device)
DECLARE_DEVICE_TYPE(R3000A,      r3000a_device)
DECLARE_DEVICE_TYPE(R3041,       r3041_device)
DECLARE_DEVICE_TYPE(R3051,       r3051_device)
DECLARE_DEVICE_TYPE(R3052,       r3052_device)
DECLARE_DEVICE_TYPE(R3052E,      r3052e_device)
DECLARE_DEVICE_TYPE(R3071,       r3071_device)
DECLARE_DEVICE_TYPE(R3081,       r3081_device)
DECLARE_DEVICE_TYPE(SONYPS2_IOP, iop_device)

#endif // MAME_CPU_MIPS_MIPS1_H
