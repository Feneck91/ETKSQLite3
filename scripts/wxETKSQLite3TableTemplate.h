$STRUCT_IMPLEMENTATION${/**
 * Structure that contains $TABLE_NAME$ datas.
 */
struct $EXPORT_IMPORT$$PREFIX_STRUCT$$TABLE_NAME$
{
    /// @name Variables used to record each row of the $TABLE_NAME$ table.
    //@{
$COLUMNS_VARIABLES_DECLARATIONS$
    //}

    /// @name Constructor.
    //@{
    /**
     * Default constructor.
     */
    $PREFIX_STRUCT$$TABLE_NAME$();

    /**
     * Destructor.
     */
    virtual ~$PREFIX_STRUCT$$TABLE_NAME$();
    //@}
};}

class $EXPORT_IMPORT$$PREFIX_TABLE$$TABLE_NAME$ : protected $PREFIX_STRUCT$$TABLE_NAME$, public ETKSQLite3Record
{
private:
    /// @name SQL string to create the $TABLE_NAME$ table into database.
    static const wxString               STR_TABLE_CONSTRUCTION;

public:
    /**
     * Typedef of the data structure that contains all table datas fields.
     *
     * Mandatory because it is used by ResultSet template class.
     */
    typedef $PREFIX_STRUCT$$TABLE_NAME$ tdTypeStruct;

    /// @name Name of the $TABLE_NAME$ table.
    static const wxString               TABLE_NAME;

    /// @name Columns of the $TABLE_NAME$ table.
    //@{
$COLUMNS_DECLARATIONS$
    //}

public:
    /// @name Constructor / Destructor.
    //@{
    /**
     * Default constructor.
     */
    $PREFIX_TABLE$$TABLE_NAME$();

    /**
     * Constructor with data struct as parameter.
     *
     * @param _rDatas Structure that contains datas.
     */
    $PREFIX_TABLE$$TABLE_NAME$(const $PREFIX_STRUCT$$TABLE_NAME$ &_rDatas);

    /**
     * Default destructor.
     */
    virtual ~$PREFIX_TABLE$$TABLE_NAME$();
    //@}

    /**
     * Get the structure that contains data.
     *
     * This structure can only be read, not modified.
     *
     * @return The base const structure class.
     */
    const tdTypeStruct &                GetStruct() const;

    /// @name Operators.
    //@{
    /**
     * Copy values into this.
     *
     * Only values are copied. Very important function, else it create new instance of this before assign to
     * this and lost columns binding (Null columns are lost) because new instance of internal ETKSQLite3ValueBindBase
     * are created.
     *
     * @param $PREFIX_ARGUMENTS_REFERENCE$Value Structure to copy into this (only values).
     * @return a reference to this.
     */
    const $PREFIX_TABLE$$TABLE_NAME$ &  operator=(const $PREFIX_STRUCT$$TABLE_NAME$ &$PREFIX_ARGUMENTS_REFERENCE$Value);

    /**
     * Cast operator.
     *
     * Allow to access directly to the struct. Important when copy the data from the record.
     *
     * @param $PREFIX_ARGUMENTS_REFERENCE$Value Structure to copy into this (only values).
     * @return a reference to this.
     */
    operator const $PREFIX_STRUCT$$TABLE_NAME$ &() const;
    //}

    /// @name Getter and setter for each row of the $TABLE_NAME$ table.
    //@{
$DECLARE_VARIABLES_GETTER_SETTER$
    //}

    /**
     * Get the SQL to be able to construct the table $TABLE_NAME$ into database.
     *
     * Be careful, when creating the database, the order of table creation is very
     * important (due to foreign key), else it can failed when the database schema is created.
     *
     * @return The SQL string that could be used to construct the table into the database.
     */
    static wxString                     GetSQLTableConstruction();
};
