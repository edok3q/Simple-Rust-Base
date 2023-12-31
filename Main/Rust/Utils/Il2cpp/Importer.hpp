#pragma once
#include "Managed.hpp"

/*
Source by: https://github.com/clxxe
Modify by: https://github.com/edok3q
If you used this source. Don't forget to credit me as an author
*/

#define FUNC( mod, type ) reinterpret_cast< decltype( &type ) >( importer::get_exported_function( importer::get_importer_module( ##mod ), #type ) )
#define TYPE( mod, type ) reinterpret_cast< type >( importer::get_exported_function( importer::get_importer_module( ##mod ), #type ) );

namespace importer {
#define contains_record( address, type, field ) ( ( type* )( ( char* )( address ) - ( std::uintptr_t )( & ( (type* ) 0 ) -> field ) ) )
#define loword(l) ((std::uint16_t)(((std::uintptr_t)(l)) & 0xffff))

	struct importer_module_t
	{
		void* base;
		const wchar_t* name;
		size_t size;
		importer_module_t( void* b, const wchar_t* n, size_t s ) : base( b ), name( n ), size( s ) {}
		importer_module_t( ) : base( 0 ), name( L"" ), size( 0 ) {}
	};

	namespace nt
	{
		struct list_entry
		{
			struct list_entry* Flink;
			struct list_entry* Blink;
		};

		struct unicode_string
		{
			unsigned short Length;
			unsigned short MaximumLength;
			wchar_t* Buffer;
		};

		struct peb_ldr_data
		{
			unsigned long Length;
			unsigned long Initialized;
			const char* SsHandle;
			list_entry InLoadOrderModuleList;
			list_entry InMemoryOrderModuleList;
			list_entry InInitializationOrderModuleList;
		};

		struct peb
		{
			unsigned char   Reserved1[ 2 ];
			unsigned char   BeingDebugged;
			unsigned char   Reserved2[ 1 ];
			const char* Reserved3[ 2 ];
			peb_ldr_data* Ldr;
		};

		struct ldr_data_table_entry
		{
			list_entry InLoadOrderModuleList;
			list_entry InMemoryOrderLinks;
			list_entry InInitializationOrderModuleList;
			void* DllBase;
			void* EntryPoint;
			union {
				unsigned long SizeOfImage;
				const char* _dummy;
			};
			unicode_string FullDllName;
			unicode_string BaseDllName;
		};

		struct image_dos_header
		{
			unsigned short e_magic;
			unsigned short e_cblp;
			unsigned short e_cp;
			unsigned short e_crlc;
			unsigned short e_cparhdr;
			unsigned short e_minalloc;
			unsigned short e_maxalloc;
			unsigned short e_ss;
			unsigned short e_sp;
			unsigned short e_csum;
			unsigned short e_ip;
			unsigned short e_cs;
			unsigned short e_lfarlc;
			unsigned short e_ovno;
			unsigned short e_res[ 4 ];
			unsigned short e_oemid;
			unsigned short e_oeminfo;
			unsigned short e_res2[ 10 ];
			long e_lfanew;
		};

		struct image_file_header
		{
			unsigned short Machine;
			unsigned short NumberOfSections;
			unsigned long TimeDateStamp;
			unsigned long PointerToSymbolTable;
			unsigned long NumberOfSymbols;
			unsigned short SizeOfOptionalHeader;
			unsigned short Characteristics;
		};

		struct image_export_directory
		{
			unsigned long Characteristics;
			unsigned long TimeDateStamp;
			unsigned short MajorVersion;
			unsigned short MinorVersion;
			unsigned long Name;
			unsigned long Base;
			unsigned long NumberOfFunctions;
			unsigned long NumberOfNames;
			unsigned long AddressOfFunctions;
			unsigned long AddressOfNames;
			unsigned long AddressOfNameOrdinals;
		};

		struct image_data_directory
		{
			unsigned long VirtualAddress;
			unsigned long Size;
		};

		struct image_optional_header
		{
			unsigned short Magic;
			unsigned char MajorLinkerVersion;
			unsigned char MinorLinkerVersion;
			unsigned long SizeOfCode;
			unsigned long SizeOfInitializedData;
			unsigned long SizeOfUninitializedData;
			unsigned long AddressOfEntryPoint;
			unsigned long BaseOfCode;
			unsigned long long ImageBase;
			unsigned long SectionAlignment;
			unsigned long FileAlignment;
			unsigned short MajorOperatingSystemVersion;
			unsigned short MinorOperatingSystemVersion;
			unsigned short MajorImageVersion;
			unsigned short MinorImageVersion;
			unsigned short MajorSubsystemVersion;
			unsigned short MinorSubsystemVersion;
			unsigned long Win32VersionValue;
			unsigned long SizeOfImage;
			unsigned long SizeOfHeaders;
			unsigned long CheckSum;
			unsigned short Subsystem;
			unsigned short DllCharacteristics;
			unsigned long long SizeOfStackReserve;
			unsigned long long SizeOfStackCommit;
			unsigned long long SizeOfHeapReserve;
			unsigned long long SizeOfHeapCommit;
			unsigned long LoaderFlags;
			unsigned long NumberOfRvaAndSizes;
			image_data_directory DataDirectory[ 16 ];
		};

		struct image_nt_headers {
			unsigned long Signature;
			image_file_header FileHeader;
			image_optional_header OptionalHeader;
		};
	}

	inline int wcsicmp_insensitive(const wchar_t* cs, const wchar_t* ct)
	{
		const auto len = managed_system::wcslen(cs);
		if (len != managed_system::wcslen(ct))
			return false;

		for (size_t i = 0; i < len; i++)
			if ((cs[i] | L' ') != (ct[i] | L' '))
				return false;

		return true;
	}

	inline auto get_importer_module( const wchar_t* name ) -> const importer::importer_module_t
	{

		const importer::nt::peb* peb = reinterpret_cast<importer::nt::peb* >( __readgsqword( 0x60 ) );
		if ( !peb )
			return {}; 

		const importer::nt::list_entry head = peb->Ldr->InMemoryOrderModuleList;

		for ( auto curr = head; curr.Flink != &peb->Ldr->InMemoryOrderModuleList; curr = *curr.Flink )
		{
			importer::nt::ldr_data_table_entry* mod = reinterpret_cast<importer::nt::ldr_data_table_entry* >( contains_record( curr.Flink, importer::nt::ldr_data_table_entry, InMemoryOrderLinks ) );

			if ( mod->BaseDllName.Buffer )
			{
				if ( wcsicmp_insensitive( mod->BaseDllName.Buffer, name ) )
				{
					return importer::importer_module_t{ mod->DllBase, mod->BaseDllName.Buffer, mod->SizeOfImage };
				}
			}
		}

		return {};
	}

	inline bool strcmp(const char* a, const char* b) {
		if ((uintptr_t)a == 0x00000000ffffffff || (uintptr_t)b == 0x00000000ffffffff)
			return false;
		if ((uintptr_t)a == 0x000000000000007d || (uintptr_t)b == 0x000000000000007d)
			return false;

		if (!a || !b)
			return !a && !b;

		int ret = 0;
		unsigned char* p1 = (unsigned char*)a;
		unsigned char* p2 = (unsigned char*)b;
		while (!(ret = *p1 - *p2) && *p2)
			++p1, ++p2;

		return ret == 0;
	}


	inline auto get_exported_function( const importer::importer_module_t module, const char* function ) -> const void*
	{
		if ( !module.base )
			return {};

		const auto base = reinterpret_cast< std::uintptr_t >( module.base );
		const importer::nt::image_dos_header* dos_header = reinterpret_cast<importer::nt::image_dos_header* >( base );
		const importer::nt::image_nt_headers* nt_header = reinterpret_cast<importer::nt::image_nt_headers* >( base + dos_header->e_lfanew );

		const importer::nt::image_data_directory* data_directory = reinterpret_cast< const importer::nt::image_data_directory* >( &nt_header->OptionalHeader.DataDirectory[ 0 ] );
		const importer::nt::image_export_directory* image_export_directory = reinterpret_cast<importer::nt::image_export_directory* >( base + data_directory->VirtualAddress );

		const auto* const rva_table = reinterpret_cast< const unsigned long* >( base + image_export_directory->AddressOfFunctions );
		const auto* const ord_table = reinterpret_cast< const unsigned short* >( base + image_export_directory->AddressOfNameOrdinals );

		if ( image_export_directory )
		{
			for ( unsigned int idx{ 0 }; idx < image_export_directory->NumberOfNames; idx++ )
			{
				const auto fn_name = reinterpret_cast< const char* >( base + reinterpret_cast< const unsigned long* >( base + image_export_directory->AddressOfNames )[ idx ] );

				if ( strcmp( fn_name, function ) )
					return reinterpret_cast< void* >( base + ( ( std::uint32_t* )( base + image_export_directory->AddressOfFunctions ) )[ ord_table[ idx ] ] );
			}
		}

		return { };
	}
}