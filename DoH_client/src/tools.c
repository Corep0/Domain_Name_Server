#include "dns.h"
static size_t ft_strlen(const char* s){
    if (!s)
        return 0;
    size_t c = 0;
    for (size_t i = 0; s[i]; i++)
        c++;
    return c;
}
size_t count_char(char *s, char c)
{
    int count = 0;
    for (size_t i = 0; s[i]; i++){
        if (s[i] == c)
            count++;
    }
    return count;
}

static int len_int(long int n){
    int c = 0;
    if (n == 0)
        return 1;
    if (n < 0)
        c++;
    while (n != 0){
        n /= 10;
        c++;
    }
    return c;
}

char *ft_itoa(int n){
    long int num = n;
    int len = len_int(num);
    char *res = malloc(sizeof(char)*(len+1));
    if (!res)
        return NULL;
    res[len] = '\0';
    if (num == 0)
        res[0] = '0';
    if (num < 0){
        res[0] = '-';
        num *= -1;
    }
    while (len-- && num != 0){
        res[len] = num % 10 + '0';
        num /= 10;
    }
    return res;
}
size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	j;
	size_t	k;

	j = 0;
	i = ft_strlen(dest);
	k = ft_strlen(src);
	if (size <= i)
		return (size + k);
	while (src[j] != '\0' && i + j < size - 1)
	{
		dest[i + j] = src[j];
		j++;
	}
	if (i <= size)
		dest[i + j] = '\0';
	return (i + k);
}
static int	count_words(const char *s, char c)
{
	size_t	i;
	int		count;
	int		in_w;

	count = 0;
	in_w = 0;
	i = 0;
	if (!s)
		return (0);
	while (s[i])
	{
		if (s[i] != c && in_w == 0)
		{
			count++;
			in_w = 1;
		}
		if (s[i] == c)
			in_w = 0;
		i++;
	}
	return (count);
}

char	*ft_substr(char *s, unsigned int start, size_t len)
{
	unsigned int	i;
	char			*str;
	size_t			s_len;

	i = 0;
	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len)
		return (strdup(""));
	if (len > s_len - start)
		len = s_len - start;
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	while (i < len && s[start + i])
	{
		str[i] = s[start + i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

static	char	*ft_extract( char *s, char c, size_t *i)
{
	size_t	start;

	if (!s)
		return (NULL);
	while (s[*i] == c)
		(*i)++;
	start = *i;
	while (s[*i] && s[*i] != c)
		(*i)++;
	return (ft_substr(s, start, *i - start));
}

static void	dest_null(char **dest, int words)
{
	while (words--)
		free(dest[words]);
	free(dest);
}

char	**ft_split( char *s, char c)
{
	size_t	i;
	int		words;
	int		index;
	char	**dest;

	words = count_words(s, c);
	index = 0;
	i = 0;
	dest = malloc((words + 1) * sizeof(char *));
	if (!s || !dest)
		return (NULL);
	while (index < words)
	{
		dest[index] = ft_extract(s, c, &i);
		if (!dest[index++])
		{
			dest_null(dest, index - 1);
			return (NULL);
		}
	}
	dest[index] = NULL;
	return (dest);
}

void print_hexdump(const void* data, int size) {
	char ascii[size +1];
	int i;

	ascii[size] = '\0';
	printf("Print_hexdump\n");
	for (i = 0; i < size; ++i) {
		if ((i+1) % 8 == 0 || i+1 == size)
			printf("\n");
		printf("%02X ", ((unsigned char*)data)[i]);
		
		if (((unsigned char*)data)[i] >= ' ' && ((unsigned char*)data)[i] <= '~')
			ascii[i % size] = ((unsigned char*)data)[i];
		else
			ascii[i % size] = ' ';
		if ((i+1) % 8 == 0 || i+1 == size) {
			printf(" ");
			if ((i+1) % size == 0) {
				printf("| ");
				for (int i = 0; i < size; i++){
					printf("%c ", ascii[i]);
				}
				printf("|  %s \n", ascii);
				return;
			}
		}
	}
}

void	print_response(uint8_t* response, int rlen){
	for (int i = 0; i < rlen; i++){
        printf("%02X --> %d ", response[i], response[i]);
        if ((char)response[i] >= ' ' && (char)response[i] <= 126)
            printf(" => %c ", (char)response[i]);
        printf("\n");
        if (i % 4 == 0)
            printf("\n");
    }
}

size_t 	tablen(char** data){
	size_t i = 0;
	for (; data[i] != NULL; i++)
		;
	return i;
}
const unsigned char *	tab_search(const unsigned char *data, unsigned int len){
	if (len == 2 && memcmp(data, "h2", 2) == 0)
		return data;
	return NULL;
}