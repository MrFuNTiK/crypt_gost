include (TestBigEndian)

function(determine_byte_ordering)
  TEST_BIG_ENDIAN(IS_BIG_ENDIAN)
  if(IS_BIG_ENDIAN)
    add_compile_definitions(CRYPT_GOST_HAS_BYTE_ORDERING CRYPT_GOST_BIG_ENDIAN)
  else()
    add_compile_definitions(CRYPT_GOST_HAS_BYTE_ORDERING CRYPT_GOST_LITTLE_ENDIAN)
  endif()
endfunction()
