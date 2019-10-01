/* libbfd.h -- Declarations used by bfd library implementation.
   This include file is not for users of the library */

/* Copyright (C) 1990, 1991 Free Software Foundation, Inc.

This file is part of BFD, the Binary File Diddler.

BFD is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 1, or (at your option)
any later version.

BFD is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with BFD; see the file COPYING.  If not, write to
the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.  */


/* If you want to read and write large blocks, you might want to do it
   in quanta of this amount */
#ifdef __HIGHC__
#define DEFAULT_BUFFERSIZE 3072 /* Metaware has 4K limit on local stack */ 
#else
#define DEFAULT_BUFFERSIZE 8192
#endif

/* Set a tdata field.  Can't use the other macros for this, since they
   do casts, and casting to the left of assignment isn't portable.  */
#define set_tdata(bfd, v) ((bfd)->tdata = (PTR) (v))

/* tdata for an archive.  For an input archive, cache
   needs to be free()'d.  For an output archive, symdefs do.  */

struct artdata {
  file_ptr first_file_filepos;
  /* Speed up searching the armap */
  struct ar_cache *cache;
  bfd *archive_head;            /* Only interesting in output routines */
  carsym *symdefs;		/* the symdef entries */
  symindex symdef_count;             /* how many there are */
  char *extended_names;		/* clever intel extension */
};

#define bfd_ardata(bfd) ((struct artdata *) ((bfd)->tdata))

/* Goes in bfd's arelt_data slot */
struct areltdata {
  char * arch_header;			     /* it's actually a string */
  unsigned int parsed_size;     /* octets of filesize not including ar_hdr */
  char *filename;			     /* null-terminated */
};

#define arelt_size(bfd) (((struct areltdata *)((bfd)->arelt_data))->parsed_size)

PROTO(PTR,  __bfd_alloc, (bfd *abfd, bfd_size_type size, char *file, int line));
PROTO(PTR,  __bfd_zalloc,(bfd *abfd, bfd_size_type size, char *file, int line));
PROTO(PTR,  __bfd_realloc,(bfd *abfd, PTR orig, bfd_size_type new, char *file, int line));
PROTO(void, __bfd_release, (bfd *abfd, PTR ptr, char *file, int line));

#define bfd_alloc(a,b)     __bfd_alloc(a,b,__FILE__,__LINE__)
#define bfd_zalloc(a,b)    __bfd_zalloc(a,b,__FILE__,__LINE__)
#define bfd_realloc(a,b,c) __bfd_realloc(a,b,c,__FILE__,__LINE__)
#define bfd_release(a,b)   __bfd_release(a,b,__FILE__,__LINE__)

PROTO (bfd_target *, bfd_find_target, (CONST char *target_name, bfd *));
PROTO (bfd_size_type, bfd_read, (PTR ptr, bfd_size_type size, bfd_size_type nitems, bfd *abfd));
PROTO (bfd_size_type, bfd_write, (PTR ptr, bfd_size_type size, bfd_size_type nitems, bfd *abfd));

PROTO (FILE *, bfd_cache_lookup, (bfd *));
PROTO (void, bfd_cache_close, (bfd *));
PROTO (int, bfd_seek,(bfd* const abfd, file_ptr fp , const int direction));
PROTO (long, bfd_tell, (bfd *abfd));
PROTO (bfd *, _bfd_create_empty_archive_element_shell, (bfd *obfd));
PROTO (bfd *, look_for_bfd_in_cache, (bfd *arch_bfd, file_ptr i));
PROTO (boolean, _bfd_generic_mkarchive, (bfd *abfd));
PROTO (struct areltdata *, snarf_ar_hdr, (bfd *abfd));
PROTO (bfd_target *, bfd_generic_archive_p, (bfd *abfd));
PROTO (boolean, bfd_slurp_bsd_armap, (bfd *abfd));
PROTO (boolean, bfd_slurp_coff_armap, (bfd *abfd));
PROTO (boolean, _bfd_slurp_extended_name_table, (bfd *abfd));
PROTO (boolean, _bfd_write_archive_contents, (bfd *abfd));
PROTO (bfd *, new_bfd, ());

#define DEFAULT_STRING_SPACE_SIZE 0x2000
PROTO (boolean, bfd_add_to_string_table, (char **table, char *new_string,
					  unsigned int *table_length,
					  char **free_ptr));
PROTO (int, _do_getb64, (unsigned char *addr,bfd_64_type *val));     
PROTO (int, _do_getl64, (unsigned char *addr,bfd_64_type *val));     
PROTO (unsigned int, _do_getb32, (unsigned char *addr));
PROTO (unsigned int, _do_getl32, (unsigned char *addr));
PROTO (unsigned int, _do_getb16, (unsigned char *addr));
PROTO (unsigned int, _do_getl16, (unsigned char *addr));
PROTO (void, _do_putb64, (bfd_64_type data, unsigned char *addr));
PROTO (void, _do_putl64, (bfd_64_type data, unsigned char *addr));
PROTO (void, _do_putb32, (unsigned long data, unsigned char *addr));
PROTO (void, _do_putl32, (unsigned long data, unsigned char *addr));
PROTO (void, _do_putb16, (int data, unsigned char *addr));
PROTO (void, _do_putl16, (int data, unsigned char *addr));

PROTO (boolean, bfd_false, (bfd *ignore));
PROTO (boolean, bfd_true, (bfd *ignore));
PROTO (PTR, bfd_nullvoidptr, (bfd *ignore));
PROTO (int, bfd_0, (bfd *ignore));
PROTO (unsigned int, bfd_0u, (bfd *ignore));
PROTO (void, bfd_void, (bfd *ignore));

PROTO (bfd *,new_bfd_contained_in,(bfd *));
PROTO (boolean, _bfd_dummy_new_section_hook, (bfd *ignore, asection *newsect));
PROTO (char *, _bfd_dummy_core_file_failing_command, (bfd *abfd));
PROTO (int, _bfd_dummy_core_file_failing_signal, (bfd *abfd));
PROTO (boolean, _bfd_dummy_core_file_matches_executable_p, (bfd *core_bfd,
							    bfd *exec_bfd));
PROTO (bfd_target *, _bfd_dummy_target, (bfd *abfd));

PROTO (void, bfd_dont_truncate_arname, (bfd *abfd, CONST char *filename,
					char *hdr));
PROTO (void, bfd_bsd_truncate_arname, (bfd *abfd, CONST char *filename,
					char *hdr));
PROTO (void, bfd_gnu_truncate_arname, (bfd *abfd, CONST char *filename,
					char *hdr));

PROTO (boolean, bsd_write_armap, (bfd *arch, unsigned int elength,
				  struct orl *map, int orl_count, int stridx));

PROTO (boolean, coff_write_armap, (bfd *arch, unsigned int elength,
				   struct orl *map, int orl_count, int stridx));

PROTO (bfd *, bfd_generic_openr_next_archived_file, (bfd *archive,
						     bfd *last_file));

PROTO(int, bfd_generic_stat_arch_elt, (bfd *, PTR));

PROTO(boolean, bfd_generic_get_section_contents,
      (bfd *abfd, sec_ptr section, PTR location, file_ptr offset, bfd_size_type count));

/* Macros to tell if bfds are read or write enabled.

   Note that bfds open for read may be scribbled into if the fd passed
   to bfd_fdopenr is actually open both for read and write
   simultaneously.  However an output bfd will never be open for
   read.  Therefore sometimes you want to check bfd_read_p or
   !bfd_read_p, and only sometimes bfd_write_p.
*/

#define bfd_read_p(abfd)      ((abfd)->direction == read_direction || (abfd)->direction == both_direction)
#define bfd_read_only_p(abfd) ((abfd)->direction == read_direction)
#define bfd_write_p(abfd)     ((abfd)->direction == write_direction || (abfd)->direction == both_direction)

PROTO (void, bfd_assert,(char*,int));
#define BFD_ASSERT(x) \
{ if (!(x)) bfd_assert(__FILE__,__LINE__); }

#define BFD_FAIL() \
{ bfd_assert(__FILE__,__LINE__); }

PROTO (FILE *, bfd_cache_lookup_worker, (bfd *));

extern bfd *bfd_last_cache;
#define bfd_cache_lookup(x) \
     (x==bfd_last_cache?(FILE*)(bfd_last_cache->iostream):bfd_cache_lookup_worker(x))
    
/* Now Steve, what's the story here? */
#ifdef lint
#define itos(x) "l"
#define stoi(x) 1
#else
#define itos(x) ((char*)(x))
#define stoi(x) ((int)(x))
#endif

/* Generic routine for close_and_cleanup is really just bfd_true.  */
#define	bfd_generic_close_and_cleanup	bfd_true

PROTO (FILE *, bfd_cache_lookup_worker, (bfd *));

PROTO (void, _bfd_add_bfd_ghist_info,
       (bfd_ghist_info **,unsigned int *,unsigned int *,
	unsigned int,CONST char *,CONST char *,int));

PROTO (int, _bfd_cmp_bfd_ghist_info, (bfd_ghist_info *,bfd_ghist_info *));

#define _BFD_GHIST_INFO_FILE_UNKNOWN "unknown"

PROTO (char *, _bfd_buystring, (char *));

PROTO (CONST char *, _bfd_trim_under_and_slashes, (CONST char *,int));

PROTO (void, _bfd_remove_ghist_info_element,
       (bfd_ghist_info *,unsigned int,unsigned int *));
