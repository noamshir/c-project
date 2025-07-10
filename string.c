void delete_white_spaces(char *str)
{
    char *temp = str;
    do
    {
        while (*temp == ' ' || *temp == '\t' || *temp == '\n')
        {
            ++temp;
        }
    } while ((*str++ = *temp++));
}
