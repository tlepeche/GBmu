#ifndef MEMORY_HPP
#define MEMORY_HPP

class Memory {

	public:
		static void			Init(uint32_t size);
		static Memory		&Instance(void);
		void				reset(void);
		uint8_t				read_byte(uint32_t addr);
		void				write_byte(uint32_t addr, uint8_t val);
		uint16_t			read_word(uint32_t addr);
		void				write_word(uint32_t addr, uint16_t val);

	private:
		static Memory		_instance;
		uint32_t			_memory_size;
		char				*_memory;
		uint32_t			_mask;

		Memory(void);
		~Memory(void);
		void				SetMemory(uint32_t size);
};

#endif
